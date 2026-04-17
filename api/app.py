"""
api/app.py
----------
FastAPI server — all LLM calls are routed through a local Ollama instance.
No local model weights are loaded; this process is a thin HTTP proxy
that formats prompts, calls Ollama, and returns structured responses.

Models are configured in configs/model.yaml:
  stage1.model  — used by /generate      (C code → LLR)
  stage2.model  — used by /rewrite       (LLR → Jama HLR)

Endpoints
---------
GET  /health          — liveness check (also verifies Ollama is reachable)
GET  /models          — list models available in Ollama
GET  /config          — show active model config
POST /generate        — two-prompt LLR generation call (Stage 1: code → LLR)
POST /rewrite         — rewrite LLRs into Jama-compliant HLRs (Stage 2)
POST /reset_stage1    — evict and reload the Stage 1 model for a clean cache

Consistency note
----------------
qwen3-coder:30b is a NON-THINKING-ONLY MoE model.  It does NOT emit
<think>…</think> blocks, and passing `think=False` to Ollama is a no-op on
it.  (Older comments in this file claimed the opposite — they were wrong
and have been removed.)

The real source of Stage 1 inconsistency was KV-cache / slot state on the
Ollama server: a freshly-started server produced pristine results, and
quality degraded on subsequent runs because cached prompt prefixes from
prior requests contaminated the attention state for later requests.

Fixes applied here:
  1. Stage 1 uses a short `keep_alive` (not -1) so Ollama can recycle slots
     between runs.  Stage 2 stays pinned because its prompts are small.
  2. `num_keep=0` is set in Stage 1 options so Ollama does not retain any
     tokens from prior requests when a new one arrives.
  3. POST /reset_stage1 unloads the Stage 1 model (keep_alive=0) and then
     reloads it with the correct num_ctx.  The Streamlit UI calls this
     before each "Generate LLRs" batch, so every run starts from the same
     pristine cache state that `./run.sh` gives on a fresh server start.
"""

from __future__ import annotations

import logging
import os
import sys
import time
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
# keep_alive policy
# ---------------------------------------------------------------------------
# Stage 1 uses a short TTL so Ollama recycles the slot between batches.
# Long runs against the same server process otherwise accumulate cached
# prompt prefixes that subtly shift attention state on later requests,
# causing the "first run is best, quality degrades" symptom.
#
# Stage 2 stays pinned (-1) because its prompts are short and the model is
# small — no cache-contamination risk there, and keeping it hot avoids the
# reload cost during back-to-back Jama rewrites.
KEEP_ALIVE_S1 = "5m"
KEEP_ALIVE_S2 = -1

# ---------------------------------------------------------------------------
# Generation options
# ---------------------------------------------------------------------------

# Stage 1 — C code → LLR
#
# Decoding settings:
#   temperature=0.0 + top_k=1 + seed=42   — greedy decoding, eliminates
#                                           sampling randomness.
#   num_ctx=16384                         — must be set explicitly; Ollama
#                                           defaults to 2048–4096 and will
#                                           silently truncate large C
#                                           functions otherwise.
#   num_keep=0                            — do NOT retain any tokens from
#                                           prior requests in this slot.
#                                           This is the options-side half
#                                           of the cache-contamination fix;
#                                           /reset_stage1 is the other half.
OLLAMA_OPTIONS_S1 = {
    "temperature":    0.0,    # greedy decoding
    "seed":           42,
    "top_p":          1.0,
    "top_k":          1,
    "num_predict":    4096,   # max output tokens
    "repeat_penalty": 1.1,
    "num_ctx":        16384,  # prompt + output window — fits large C functions
    "num_keep":       0,      # don't retain tokens across requests
}

# Stage 2 — LLR → Jama-compliant LLR
# Prompts are much shorter (just LLR text), so a smaller context is fine.
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
    version="1.1.0",
)

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_methods=["*"],
    allow_headers=["*"],
)


# ---------------------------------------------------------------------------
# Model (un)loading helpers
# ---------------------------------------------------------------------------
# Ollama's default context window is 2048–4096 tokens.  If the Stage 1 model
# is already resident in memory from a previous session (or from an `ollama
# run` in the terminal), it may have been loaded with a SMALLER num_ctx.
# When our API then sends a request with num_ctx=16384, Ollama sometimes
# does NOT resize the already-loaded model — it silently truncates the
# prompt to whatever context window it was originally loaded with.
#
# _preload_model forces Ollama to (re)load the model with the correct
# num_ctx before the first real request arrives.
# ---------------------------------------------------------------------------

def _preload_model(model: str, options: dict, keep_alive=-1) -> None:
    """Load a model into Ollama with the correct num_ctx, without generating.

    Uses num_predict=0 so Ollama loads the model and allocates the KV cache
    at the requested num_ctx, but produces zero output tokens.  This avoids
    contaminating the KV cache slot with generated tokens from a dummy prompt
    — a subtle bug that caused the first real generation to be worse than
    the second (the preload's output tokens sat in the slot and shifted
    attention state for the first real request).
    """
    url = f"{OLLAMA_BASE_URL.rstrip('/')}/api/generate"
    # Merge num_predict=0 into the options so the model loads but doesn't
    # generate anything.  We keep the caller's options intact.
    load_options = {**options, "num_predict": 0}
    payload = {
        "model":      model,
        "prompt":     "",             # empty — nothing to process
        "stream":     False,
        "keep_alive": keep_alive,
        "options":    load_options,
    }
    try:
        resp = requests.post(url, json=payload, timeout=120)
        resp.raise_for_status()
        logger.info("Preloaded model '%s' with num_ctx=%s", model, options.get("num_ctx"))
    except Exception as exc:
        logger.warning("Failed to preload model '%s': %s (will load on first request)", model, exc)


def _unload_model(model: str) -> None:
    """Tell Ollama to evict a model from memory immediately (keep_alive=0)."""
    url = f"{OLLAMA_BASE_URL.rstrip('/')}/api/generate"
    payload = {"model": model, "keep_alive": 0}
    try:
        resp = requests.post(url, json=payload, timeout=30)
        resp.raise_for_status()
        logger.info("Unloaded model '%s' from Ollama memory", model)
    except Exception as exc:
        logger.warning("Failed to unload model '%s': %s", model, exc)


@app.on_event("startup")
def startup_preload_models() -> None:
    """
    Preload the Stage 1 model on server startup so it is resident in GPU
    memory with the CORRECT context window size before any user request.

    Stage 2 is NOT preloaded here — it loads on-demand when /rewrite is
    first called.  Loading both models at startup would force them to
    share VRAM, and qwen3-coder:30b needs most of a 24 GB GPU to itself
    (weights + KV cache at num_ctx=16384).  Co-loading Stage 2 can cause
    Ollama to partially offload Stage 1 to CPU, silently degrading quality.
    """
    logger.info("Preloading Stage 1 model '%s' (num_ctx=%d)…",
                STAGE1_MODEL, OLLAMA_OPTIONS_S1["num_ctx"])
    _preload_model(STAGE1_MODEL, OLLAMA_OPTIONS_S1, keep_alive=KEEP_ALIVE_S1)
    logger.info("Model preload complete. Stage 2 ('%s') will load on first /rewrite call.",
                STAGE2_MODEL)


# ---------------------------------------------------------------------------
# Shared Ollama helper
# ---------------------------------------------------------------------------

def _ollama_generate(
    prompt:     str,
    model:      str,
    options:    dict = None,
    keep_alive = KEEP_ALIVE_S1,
) -> str:
    """Send a single prompt to Ollama and return the text response."""
    url = f"{OLLAMA_BASE_URL.rstrip('/')}/api/generate"
    payload = {
        "model":      model,
        "prompt":     prompt,
        "stream":     False,
        "keep_alive": keep_alive,
        "options":    options or OLLAMA_OPTIONS_S1,
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


class ResetResponse(BaseModel):
    status: str
    model:  str


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
        "ollama_base_url":   OLLAMA_BASE_URL,
        "stage1_model":      STAGE1_MODEL,
        "stage2_model":      STAGE2_MODEL,
        "stage1_options":    OLLAMA_OPTIONS_S1,
        "stage2_options":    OLLAMA_OPTIONS_S2,
        "stage1_keep_alive": KEEP_ALIVE_S1,
        "stage2_keep_alive": KEEP_ALIVE_S2,
    }


@app.post("/reset_stage1", response_model=ResetResponse)
def reset_stage1():
    """
    Evict ALL models from Ollama's memory, then reload only Stage 1.

    This guarantees two things:
      1. A pristine KV cache (no leftover state from prior requests).
      2. Full GPU residency for Stage 1 — no VRAM competition from the
         Stage 2 model.  On a 24 GB GPU, qwen3-coder:30b (~17 GB weights
         + KV cache at num_ctx=16384) leaves little room for a co-resident
         model.  If Stage 2 is loaded when Stage 1 starts a batch, Ollama
         may partially offload Stage 1 to CPU, silently degrading quality.

    The Streamlit UI calls this before every "Generate LLRs" batch.
    Stage 2 will be reloaded on-demand when /rewrite is first called.
    """
    # Evict Stage 2 first so its VRAM is freed before Stage 1 reloads.
    _unload_model(STAGE2_MODEL)
    _unload_model(STAGE1_MODEL)

    # Give Ollama time to release VRAM.  The unload API returns immediately
    # but the actual VRAM free is asynchronous — Ollama's scheduler polls
    # until ~90% of expected VRAM is returned.  Without this pause the
    # preload can start before the memory is fully available, causing the
    # model to load with partial GPU offload.
    time.sleep(2)

    _preload_model(STAGE1_MODEL, OLLAMA_OPTIONS_S1, keep_alive=KEEP_ALIVE_S1)
    return ResetResponse(status="reloaded", model=STAGE1_MODEL)


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
        raw = _ollama_generate(
            req.prompts[0],
            model=STAGE1_MODEL,
            options=OLLAMA_OPTIONS_S1,
            keep_alive=KEEP_ALIVE_S1,
        )
        heading = raw.split(":")[-1].strip() if ":" in raw else raw.strip()

    body = _ollama_generate(
        req.prompts[1],
        model=STAGE1_MODEL,
        options=OLLAMA_OPTIONS_S1,
        keep_alive=KEEP_ALIVE_S1,
    )
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
        raw = _ollama_generate(
            prompt,
            model=STAGE2_MODEL,
            options=OLLAMA_OPTIONS_S2,
            keep_alive=KEEP_ALIVE_S2,
        )
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
