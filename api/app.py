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

To disable the thinking pass and achieve true greedy determinism, think=False
is sent as a TOP-LEVEL parameter in the Ollama API request payload.  This is
critical — placing "think" inside the "options" dict has NO effect.

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
STAGE3_MODEL:    str = os.getenv("STAGE3_MODEL",    _cfg["stage3"]["model"])

# ---------------------------------------------------------------------------
# Generation options
# ---------------------------------------------------------------------------

# Stage 1 — C code → LLR
#
# KEY DETERMINISM SETTINGS for Qwen3 (qwen3-coder:30b and any qwen3-* model):
#
#   think=False  (top-level API parameter, NOT inside "options")
#       Disables Qwen3's internal chain-of-thought reasoning pass.
#       Without this, Qwen3 runs a non-deterministic thinking phase before
#       producing visible output, causing different requirements to be generated
#       on each run even when temperature=0 and seed=42 are set.
#
#       IMPORTANT: "think" must be a top-level key in the Ollama API request
#       payload.  Placing it inside "options" has NO effect — Ollama silently
#       ignores unknown keys in "options".
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
# Non-Qwen3 fallback: if your stage1.model is not a Qwen3 model, think=False
# is silently ignored by Ollama, so this is safe for any model.
OLLAMA_OPTIONS_S1 = {
    "temperature":    0.0,    # greedy decoding
    "seed":           42,
    "top_p":          1.0,
    "top_k":          1,
    "num_predict":    4096,   # max output tokens
    "repeat_penalty": 1.1,
    "num_ctx":        16384,  # prompt + output window — fits large C functions
}

# Stage 2 — LLR → Jama-compliant LLR
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

# Stage 3 — Jama LLR → HLR (merging related LLRs into high-level requirements)
# Same context needs as Stage 2.
OLLAMA_OPTIONS_S3 = {
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
# Model preloading
# ---------------------------------------------------------------------------
# Ollama's default context window is 2048–4096 tokens.  If the Stage 1 model
# is already resident in memory from a previous session (or from an `ollama
# run` in the terminal), it may have been loaded with a SMALLER num_ctx.
# When our API then sends a request with num_ctx=16384, Ollama sometimes
# does NOT resize the already-loaded model — it silently truncates the
# prompt to whatever context window it was originally loaded with.
#
# Running Stage 2 (which uses a different model) evicts Stage 1's model from
# GPU memory.  The NEXT Stage 1 call then does a fresh load that respects
# num_ctx=16384, producing better results.
#
# Fix: on server startup, send an empty "warm-up" request to Ollama for
# EACH stage model with the correct num_ctx and keep_alive=-1.  This forces
# Ollama to load (or reload) the model with the context window we need,
# and pins it in memory so it's never silently evicted.
# ---------------------------------------------------------------------------

def _preload_model(model: str, options: dict, think: bool | None = None):
    """Send a minimal request to Ollama to preload a model with specific options."""
    url = f"{OLLAMA_BASE_URL.rstrip('/')}/api/generate"
    payload = {
        "model":      model,
        "prompt":     " ",          # minimal prompt — just trigger a load
        "stream":     False,
        "keep_alive": -1,           # pin in memory indefinitely
        "options":    options,
    }
    if think is not None:
        payload["think"] = think
    try:
        resp = requests.post(url, json=payload, timeout=120)
        resp.raise_for_status()
        logger.info("Preloaded model '%s' with num_ctx=%s", model, options.get("num_ctx"))
    except Exception as exc:
        logger.warning("Failed to preload model '%s': %s (will load on first request)", model, exc)


@app.on_event("startup")
def startup_preload_models():
    """
    Preload Stage 1 and Stage 2 models on server startup so they are
    resident in GPU memory with the CORRECT context window sizes before
    any user request arrives.
    """
    logger.info("Preloading Stage 1 model '%s' (num_ctx=%d)…", STAGE1_MODEL, OLLAMA_OPTIONS_S1["num_ctx"])
    _preload_model(STAGE1_MODEL, OLLAMA_OPTIONS_S1, think=False)

    logger.info("Preloading Stage 2 model '%s' (num_ctx=%d)…", STAGE2_MODEL, OLLAMA_OPTIONS_S2["num_ctx"])
    _preload_model(STAGE2_MODEL, OLLAMA_OPTIONS_S2)

    logger.info("Model preload complete.")


# ---------------------------------------------------------------------------
# Shared Ollama helper
# ---------------------------------------------------------------------------

def _ollama_generate(prompt: str, model: str, options: dict = None, think: bool | None = None) -> str:
    """Send a single prompt to Ollama and return the text response."""
    url = f"{OLLAMA_BASE_URL.rstrip('/')}/api/generate"
    payload = {
        "model":      model,
        "prompt":     prompt,
        "stream":     False,
        "keep_alive": -1,       # keep model pinned in memory between stages
        "options":    options or OLLAMA_OPTIONS_S1,
    }
    # "think" must be a top-level key in the Ollama API payload, NOT inside
    # "options".  When set to False it suppresses Qwen3's non-deterministic
    # chain-of-thought reasoning pass so that temperature=0 + seed=42 produce
    # truly identical output across runs.
    if think is not None:
        payload["think"] = think
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
            "stage3_model":     STAGE3_MODEL,
            "stage1_thinking":  False,   # always disabled — think=False sent as top-level API param
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
        "stage3_model":     STAGE3_MODEL,
        "stage1_options":   OLLAMA_OPTIONS_S1,
        "stage2_options":   OLLAMA_OPTIONS_S2,
        "stage3_options":   OLLAMA_OPTIONS_S3,
    }


@app.post("/generate", response_model=GenerateResponse)
def generate(req: GenerateRequest):
    """
    Stage 1: C code → LLR.
    Uses STAGE1_MODEL (set in configs/model.yaml).
    Accepts [heading_prompt, body_prompt]; returns [heading, body].

    Qwen3 thinking is disabled via think=False (top-level API parameter),
    ensuring identical output on every call for the same input.
    """
    if len(req.prompts) < 2:
        raise HTTPException(status_code=422, detail="Expected exactly 2 prompts.")

    if req.prompts[0].strip().upper() == "N/A":
        heading = "N/A"
    else:
        raw = _ollama_generate(req.prompts[0], model=STAGE1_MODEL, options=OLLAMA_OPTIONS_S1, think=False)
        heading = raw.split(":")[-1].strip() if ":" in raw else raw.strip()

    body = _ollama_generate(req.prompts[1], model=STAGE1_MODEL, options=OLLAMA_OPTIONS_S1, think=False)
    return GenerateResponse(requirements=[heading, body], model=STAGE1_MODEL)


@app.post("/rewrite", response_model=RewriteResponse)
def rewrite(req: RewriteRequest):
    """
    Stage 2: LLR → Jama-compliant LLR.
    Uses STAGE2_MODEL (set in configs/model.yaml).

    Calls go through the shared _ollama_generate helper (with Stage 2 options)
    so that all Ollama communication is centralised in app.py rather than split
    between app.py and inference_pipeline._call_ollama.
    """
    from src.pipelines.inference_pipeline import (
        FunctionGroup, rewrite_function, _REWRITE_PROMPT, _safe_json_parse,
    )
    from src.utils.reference_loader import verification_methods_block
    import logging as _log

    group      = FunctionGroup(function_name=req.function_name, draft=req.draft)
    vm_context = verification_methods_block()

    prompt = _REWRITE_PROMPT.format(
        function_name               = group.function_name,
        draft                       = group.draft,
        verification_methods_context = vm_context,
    )

    try:
        raw    = _ollama_generate(prompt, model=STAGE2_MODEL, options=OLLAMA_OPTIONS_S2)
        result = _safe_json_parse(raw)
        if not isinstance(result, list):
            raise ValueError("LLM response is not a JSON array")
        results = result
    except Exception as exc:
        _log.warning(
            "Rewrite failed for '%s': %s — preserving original draft as fallback.",
            group.function_name, exc,
        )
        results = [{
            "name": group.function_name.replace("_", " ").title(),
            "description": group.draft,
            "verification_method": "Analysis",
            "requirement_type": "Functional",
        }]

    return RewriteResponse(
        function_name=req.function_name,
        requirements=results,
        model=STAGE2_MODEL,
    )


# ---------------------------------------------------------------------------
# Stage 3 — Jama LLR → HLR (merge related LLRs into high-level requirements)
# ---------------------------------------------------------------------------

class SynthesizeHLRRequest(BaseModel):
    """Stage 3 — Jama LLR to HLR."""
    function_name: str
    draft:         str     # concatenated Jama LLR text for this function


class SynthesizeHLRResponse(BaseModel):
    function_name: str
    requirements:  list
    model:         str


@app.post("/synthesize_hlr", response_model=SynthesizeHLRResponse)
def synthesize_hlr(req: SynthesizeHLRRequest):
    """
    Stage 3: Jama LLR → HLR.
    Uses STAGE3_MODEL (set in configs/model.yaml).

    Merges related low-level requirements into fewer high-level requirements,
    preserving all numerical values.
    """
    from src.pipelines.hlr_pipeline import (
        FunctionGroup, get_hlr_prompt, _safe_json_parse,
    )
    from src.utils.reference_loader import verification_methods_block
    import logging as _log

    group      = FunctionGroup(function_name=req.function_name, draft=req.draft)
    # vm_context = verification_methods_block()
    prompt_tpl = get_hlr_prompt()

    prompt = prompt_tpl.format(
        # function_name               = group.function_name,
        llrs_json                       = group.draft,
        # verification_methods_context = vm_context,
    )

    try:
        raw    = _ollama_generate(prompt, model=STAGE3_MODEL, options=OLLAMA_OPTIONS_S3)
        result = _safe_json_parse(raw)
        if not isinstance(result, list):
            raise ValueError("LLM response is not a JSON array")
        results = result
    except Exception as exc:
        _log.warning(
            "HLR synthesis failed for '%s': %s — preserving original as fallback.",
            group.function_name, exc,
        )
        results = [{
            "name": group.function_name.replace("_", " ").title(),
            "description": group.draft,
            "verification_method": "Analysis",
            "requirement_type": "Functional",
        }]

    return SynthesizeHLRResponse(
        function_name=req.function_name,
        requirements=results,
        model=STAGE3_MODEL,
    )
