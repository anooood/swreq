"""
prompt_logger.py
----------------
Writes full LLM prompts and raw responses to a timestamped file under
logs/prompts/  using direct file I/O — no logging-level or buffering issues.

Each run creates one file:  logs/prompts/YYYYMMDD_HHMMSS_prompts.log

Usage
-----
    from src.utils.prompt_logger import log_prompt, log_response, log_file_path

    log_prompt("rewrite", function_name, prompt)
    raw = _call_ollama(prompt, model, base_url)
    log_response("rewrite", function_name, raw)
"""

from __future__ import annotations

from datetime import datetime
from pathlib import Path

# ---------------------------------------------------------------------------
# Session file — created once on first import
# ---------------------------------------------------------------------------

_LOGS_DIR = Path(__file__).resolve().parents[2] / "logs" / "prompts"
_LOGS_DIR.mkdir(parents=True, exist_ok=True)

_session_ts = datetime.now().strftime("%Y%m%d_%H%M%S")
_log_file   = _LOGS_DIR / f"{_session_ts}_prompts.log"

# Touch the file so it exists immediately
_log_file.touch()


# ---------------------------------------------------------------------------
# Internal writer
# ---------------------------------------------------------------------------

def _write(text: str) -> None:
    with _log_file.open("a", encoding="utf-8") as f:
        f.write(text)
        f.flush()


# ---------------------------------------------------------------------------
# Public API
# ---------------------------------------------------------------------------

def log_prompt(stage: str, function_name: str, prompt: str) -> None:
    """Write a full prompt block to the session log file."""
    _write(
        f"\n{'━' * 80}\n"
        f"PROMPT   │ stage={stage}  │  function={function_name}\n"
        f"{'─' * 80}\n"
        f"{prompt}\n"
    )


def log_response(stage: str, function_name: str, response: str) -> None:
    """Write a raw LLM response block to the session log file."""
    _write(
        f"{'─' * 80}\n"
        f"RESPONSE │ stage={stage}  │  function={function_name}\n"
        f"{'─' * 80}\n"
        f"{response}\n"
        f"{'━' * 80}\n"
    )


def log_file_path() -> Path:
    """Return the path of the current session's log file."""
    return _log_file
