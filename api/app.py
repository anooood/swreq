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

Determinism note
----------------
Qwen3 models (e.g. qwen3-coder:30b) are hybrid thinking/non-thinking models.
They emit internal <think>…</think> reasoning tokens whose content is NOT
controlled by temperature=0 / seed=42, making outputs non-deterministic across
calls even with greedy settings.

To disable the thinking pass and achieve true greedy determinism:
  1. Append ":no-thinking" to the model tag in model.yaml  (Ollama ≥ 0.6.5)
     OR keep the base tag and use the "think": false option below (preferred —
     no need to pull a separate model variant).
  2. The "think": false option is passed in OLLAMA_OPTIONS_S1 below.

Stage 2 uses qwen2.5 which has no thinking mode — no change needed there.
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

# ---------------------------------------------------------------------------
# Generation options
# ---------------------------------------------------------------------------

# Stage 1 — C code → LLR
#
# KEY DETERMINISM SETTINGS for Qwen3 (qwen3-coder:30b and any qwen3-* model):
#
#   "think": false
#       Disables Qwen3's internal chain-of-thought reasoning pass.
#       Without this, Qwen3 runs a non-deterministic thinking phase before
#       producing visible output, causing different requirements to be generated
#       on each run even when temperature=0 and seed=42 are set.
#       Ollama exposes this as a first-class option (requires Ollama ≥ 0.6.5).
#
#   "temperature": 0.0 + "seed": 42 + "top_k": 1
#       Standard greedy decoding — eliminates all sampling randomness in the
#       visible (non-thinking) generation pass.
#
#   "num_ctx": 16384
#       Must be set explicitly. Ollama defaults to 2048 tokens total context
#       window, which silently truncates large C functions before generation
#       even begins, producing different truncation points across runs.
#       Mistral-7B and Qwen3-30B both support up to 32768.
#
# Non-Qwen3 fallback: if your stage1.model is not a Qwen3 model, "think": false
# is silently ignored by Ollama, so these options are safe for any model.
OLLAMA_OPTIONS_S1 = {
    "temperature":    0.0,    # greedy decoding
    "seed":           42,
    "top_p":          1.0,
    "top_k":          1,
    "num_predict":    4096,   # max output tokens
    "repeat_penalty": 1.1,
    "num_ctx":        16384,  # prompt + output window — fits large C functions
    "think":          False,  # disable Qwen3 thinking pass (no-op for other models)
}

# Stage 2 — LLR → Jama HLR
# Prompts are much shorter (just LLR text), so a smaller context is fine.
# Stage 2 uses qwen2.5 which has no thinking mode — "think" key not needed.
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
            "status":           "ok",
            "ollama":           OLLAMA_BASE_URL,
            "available_models": models,
            "stage1_model":     STAGE1_MODEL,
            "stage2_model":     STAGE2_MODEL,
            "stage1_thinking":  False,   # always disabled — see OLLAMA_OPTIONS_S1
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
        "ollama_base_url":  OLLAMA_BASE_URL,
        "stage1_model":     STAGE1_MODEL,
        "stage2_model":     STAGE2_MODEL,
        "stage1_options":   OLLAMA_OPTIONS_S1,
        "stage2_options":   OLLAMA_OPTIONS_S2,
    }


@app.post("/generate", response_model=GenerateResponse)
def generate(req: GenerateRequest):
    """
    Stage 1: C code → LLR.
    Uses STAGE1_MODEL (set in configs/model.yaml).
    Accepts [heading_prompt, body_prompt]; returns [heading, body].

    Qwen3 thinking is disabled via OLLAMA_OPTIONS_S1["think"] = False,
    ensuring identical output on every call for the same input.
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
