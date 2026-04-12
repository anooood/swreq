"""
api/app.py
----------
FastAPI server — all LLM calls are routed through a local Ollama instance.
No local model weights are loaded; this process is a thin HTTP proxy
that formats prompts, calls Ollama, and returns structured responses.

Models are configured in configs/model.yaml:
  stage1.model  — used by /generate  (C code → LLR)
  stage2.model  — used by /rewrite   (LLR → Jama HLR)

Endpoints
---------
GET  /health          — liveness check (also verifies Ollama is reachable)
GET  /models          — list models available in Ollama
GET  /config          — show active model config
POST /generate        — two-prompt LLR generation call (Stage 1: code → LLR)
POST /rewrite         — rewrite LLRs into Jama-compliant HLRs (Stage 2)
"""

from __future__ import annotations

import logging
import os
import sys
from pathlib import Path as _Path
from typing import List

import requests
import yaml
from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel

# Ensure repo root is on sys.path when uvicorn is started from any directory
_REPO_ROOT = _Path(__file__).resolve().parent.parent
if str(_REPO_ROOT) not in sys.path:
    sys.path.insert(0, str(_REPO_ROOT))

logger = logging.getLogger(__name__)

# ---------------------------------------------------------------------------
# Load model config from configs/model.yaml
# ---------------------------------------------------------------------------

def _load_model_config() -> dict:
    config_path = _REPO_ROOT / "configs" / "model.yaml"
    with open(config_path) as f:
        return yaml.safe_load(f)

_cfg = _load_model_config()

OLLAMA_BASE_URL: str = os.getenv("OLLAMA_BASE_URL", _cfg["ollama"]["base_url"])
STAGE1_MODEL:    str = os.getenv("STAGE1_MODEL",    _cfg["stage1"]["model"])
STAGE2_MODEL:    str = os.getenv("STAGE2_MODEL",    _cfg["stage2"]["model"])

# Generation options — mirrored from the original transformers GEN_CONFIG.
# num_predict  : Ollama's equivalent of max_new_tokens — set high to avoid output truncation.
# repeat_penalty: Ollama's equivalent of repetition_penalty.
# num_ctx      : CRITICAL — Ollama defaults to 2048 tokens total context window.
#                Mistral-7B supports up to 32768. Setting this high ensures long
#                C functions + globals in the prompt are never silently truncated
#                before generation even begins.
OLLAMA_OPTIONS_S1 = {
    "temperature":    0.0,    # greedy decoding (do_sample=False)
    "seed":           42,
    "top_p":          1.0,
    "top_k":          1,
    "num_predict":    4096,   # max output tokens
    "repeat_penalty": 1.1,
    "num_ctx":        16384,  # prompt + output window — fits large C functions
}

# Stage 2 prompts are much shorter (just LLR text), so a smaller context is fine.
OLLAMA_OPTIONS_S2 = {
    "temperature":    0.0,
    "seed":           42,
    "top_p":          1.0,
    "top_k":          1,
    "num_predict":    4096,
    "repeat_penalty": 1.1,
    "num_ctx":        8192,
}

# ---------------------------------------------------------------------------
# App
# ---------------------------------------------------------------------------

app = FastAPI(
    title="swreq LLM API",
    description="LLR generation and Jama-compliance rewriting via Ollama.",
    version="1.0.0",
)

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_methods=["*"],
    allow_headers=["*"],
)

# ---------------------------------------------------------------------------
# Shared Ollama helper
# ---------------------------------------------------------------------------

def _ollama_generate(prompt: str, model: str, options: dict = None) -> str:
    """Send a single prompt to Ollama and return the text response."""
    url = f"{OLLAMA_BASE_URL.rstrip('/')}/api/generate"
    payload = {
        "model":   model,
        "prompt":  prompt,
        "stream":  False,
        "options": options or OLLAMA_OPTIONS_S1,
    }
    try:
        resp = requests.post(url, json=payload, timeout=300)
        resp.raise_for_status()
    except requests.exceptions.ConnectionError:
        raise HTTPException(
            status_code=503,
            detail=f"Cannot reach Ollama at {OLLAMA_BASE_URL}. Is 'ollama serve' running?",
        )
    except requests.exceptions.HTTPError as exc:
        raise HTTPException(status_code=502, detail=f"Ollama error: {exc}")
    return resp.json().get("response", "")


# ---------------------------------------------------------------------------
# Request / Response schemas
# ---------------------------------------------------------------------------

class GenerateRequest(BaseModel):
    """Stage 1 — C code to LLR."""
    prompts: List[str]


class GenerateResponse(BaseModel):
    requirements: List[str]
    model: str


class RewriteRequest(BaseModel):
    """Stage 2 — LLR to Jama HLR."""
    function_name: str
    draft:         str


class RewriteResponse(BaseModel):
    function_name: str
    requirements:  list
    model:         str


# ---------------------------------------------------------------------------
# Endpoints
# ---------------------------------------------------------------------------

@app.get("/health")
def health():
    """Liveness + Ollama connectivity check."""
    try:
        resp = requests.get(f"{OLLAMA_BASE_URL.rstrip('/')}/api/tags", timeout=5)
        resp.raise_for_status()
        models = [m["name"] for m in resp.json().get("models", [])]
        return {
            "status":          "ok",
            "ollama":          OLLAMA_BASE_URL,
            "available_models": models,
            "stage1_model":    STAGE1_MODEL,
            "stage2_model":    STAGE2_MODEL,
        }
    except Exception as exc:
        raise HTTPException(status_code=503, detail=f"Ollama unreachable: {exc}")


@app.get("/models")
def list_models():
    """Return all models currently available in Ollama."""
    try:
        resp = requests.get(f"{OLLAMA_BASE_URL.rstrip('/')}/api/tags", timeout=5)
        resp.raise_for_status()
        return {"models": [m["name"] for m in resp.json().get("models", [])]}
    except Exception as exc:
        raise HTTPException(status_code=503, detail=str(exc))


@app.get("/config")
def get_config():
    """Show the active model configuration."""
    return {
        "ollama_base_url": OLLAMA_BASE_URL,
        "stage1_model":    STAGE1_MODEL,
        "stage2_model":    STAGE2_MODEL,
    }


@app.post("/generate", response_model=GenerateResponse)
def generate(req: GenerateRequest):
    """
    Stage 1: C code → LLR.
    Uses STAGE1_MODEL (set in configs/model.yaml).
    Accepts [heading_prompt, body_prompt]; returns [heading, body].
    """
    if len(req.prompts) < 2:
        raise HTTPException(status_code=422, detail="Expected exactly 2 prompts.")

    if req.prompts[0].strip().upper() == "N/A":
        heading = "N/A"
    else:
        raw = _ollama_generate(req.prompts[0], model=STAGE1_MODEL, options=OLLAMA_OPTIONS_S1)
        heading = raw.split(":")[-1].strip() if ":" in raw else raw.strip()

    body = _ollama_generate(req.prompts[1], model=STAGE1_MODEL, options=OLLAMA_OPTIONS_S1)
    return GenerateResponse(requirements=[heading, body], model=STAGE1_MODEL)


@app.post("/rewrite", response_model=RewriteResponse)
def rewrite(req: RewriteRequest):
    """
    Stage 2: LLR → Jama-compliant HLR.
    Uses STAGE2_MODEL (set in configs/model.yaml).
    """
    from src.pipelines.inference_pipeline import FunctionGroup, rewrite_function

    group   = FunctionGroup(function_name=req.function_name, draft=req.draft)
    results = rewrite_function(
        group,
        id_start="#001",
        model=STAGE2_MODEL,
        base_url=OLLAMA_BASE_URL,
    )
    return RewriteResponse(
        function_name=req.function_name,
        requirements=results,
        model=STAGE2_MODEL,
    )
