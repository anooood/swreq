"""
merger_rewrite.py
-----------------
Use this instead of merger.py when HLRs for each function already exist as a
combined draft string and need to be rewritten as Jama-compliant requirements.

CSV columns (case-sensitive):
  "Function Name"  — name of the source function
  "Requirements"   — all draft HLR text for that function in a single string

One LLM call per function row. The LLM:
  - Parses the draft string to identify individual obligations
  - Decides how many Jama requirements are needed (1 or more)
  - Rewrites each one as a clean, single "shall" sentence
  - Preserves ALL details from the original draft — nothing is dropped

Compatible with the same exporter.py and main.py (--mode rewrite).
"""

from __future__ import annotations

import json
import logging
from dataclasses import dataclass, field
from typing import Any

import pandas as pd
import requests

from src.utils.reference_loader import verification_methods_block, requirement_type_block
from src.utils.prompt_templates import _REWRITE_PROMPT
from src.utils.prompt_logger import log_prompt, log_response, log_file_path

logger = logging.getLogger(__name__)

CSV_FUNCTION_COL = "Function Name"
CSV_DRAFT_COL    = "Requirements"

# ---------------------------------------------------------------------------
# Ollama generation options — eliminate randomness
# ---------------------------------------------------------------------------

# Stage 2 (LLR → Jama HLR) options.
# num_ctx 8192 is sufficient — Stage 2 prompts contain only LLR text, not raw C code.
OLLAMA_OPTIONS = {
    "temperature":    0.0,   # deterministic
    "seed":           42,
    "top_p":          1.0,
    "top_k":          1,
    "num_predict":    4096,  # max output tokens
    "repeat_penalty": 1.1,
    "num_ctx":        8192,  # context window for Stage 2
}

# ---------------------------------------------------------------------------
# Data classes  (same shape as merger.py so exporter.py works unchanged)
# ---------------------------------------------------------------------------

@dataclass
class FunctionGroup:
    """One function and its combined draft HLR string."""
    function_name: str
    draft: str                  # the full combined Requirements string


@dataclass
class HighLevelRequirement:
    id: str
    function_name: str
    name: str
    description: str
    verification_method: str
    requirement_type: str
    engineering_discipline: str = "Software"

@dataclass
class MergeResult:
    hlrs: list[HighLevelRequirement]
    function_count: int = 0
    llr_count: int = 0          # total draft strings processed (= function count here)
    _groups: list[FunctionGroup] = field(default_factory=list)


# ---------------------------------------------------------------------------
# CSV parser
# ---------------------------------------------------------------------------

def parse_groups_from_csv(path: str) -> list[FunctionGroup]:
    """
    Read CSV with columns 'Function Name' and 'Requirements'.
    Each row = one function with its entire draft HLR block.
    Multiple rows with the same Function Name are concatenated.
    """
    df = pd.read_csv(path, dtype=str).fillna("")

    missing = [c for c in (CSV_FUNCTION_COL, CSV_DRAFT_COL) if c not in df.columns]
    if missing:
        raise ValueError(
            f"CSV is missing column(s): {missing}. Found: {list(df.columns)}"
        )

    groups: dict[str, list[str]] = {}
    for _, row in df.iterrows():
        func  = row[CSV_FUNCTION_COL].strip()
        draft = row[CSV_DRAFT_COL].strip()
        if not func or not draft:
            continue
        groups.setdefault(func, [])
        groups[func].append(draft)

    # Join multiple rows for the same function with a newline separator
    return [
        FunctionGroup(function_name=f, draft=" ".join(parts))
        for f, parts in groups.items()
    ]

# ---------------------------------------------------------------------------
# Ollama call
# ---------------------------------------------------------------------------

def _call_ollama(prompt: str, model: str, base_url: str) -> str:
    url  = f"{base_url.rstrip('/')}/api/generate"
    body = {"model": model, "prompt": prompt, "stream": False, "options":OLLAMA_OPTIONS}
    resp = requests.post(url, json=body, timeout=180)
    resp.raise_for_status()
    return resp.json()["response"]


def _safe_json_parse(raw: str) -> Any:
    text = raw.strip()
    if text.startswith("```"):
        lines = text.splitlines()
        end   = -1 if lines[-1].strip() == "```" else len(lines)
        text  = "\n".join(lines[1:end])
    return json.loads(text)


# ---------------------------------------------------------------------------
# Per-function rewrite  (one LLM call per function row)
# ---------------------------------------------------------------------------

def rewrite_function(
    group: FunctionGroup,
    id_start: str,
    model: str,
    base_url: str,
    vm_context: str = "",
    rt_context: str = ""
) -> list[dict]:
    """
    Sends the full draft string to the LLM and returns a list of rewritten
    Jama requirement dicts (1 or more depending on how many obligations
    the LLM identifies in the draft).
    Falls back to a single safe dict preserving the original draft on error.
    """
    prompt = _REWRITE_PROMPT.format(
        function_name = group.function_name,
        draft         = group.draft,
        id_start      = id_start,
        verification_methods_context = vm_context,
        requirement_type_context = rt_context
    )
    log_prompt("rewrite", group.function_name, prompt)
    try:
        raw    = _call_ollama(prompt, model, base_url)
        result = _safe_json_parse(raw)
        if not isinstance(result, list):
            raise ValueError("LLM response is not a JSON array")
        return result
    except Exception as exc:
        logger.warning(
            "Rewrite failed for '%s': %s — preserving original draft as fallback.",
            group.function_name, exc,
        )
        return [{
            "name": group.function_name.replace("_", " ").title(),
            "description": group.draft,          # original preserved verbatim
            "verification_method": "Analysis",
            "requirement_type": "Functional",
        }]


# ---------------------------------------------------------------------------
# Main entry point
# ---------------------------------------------------------------------------

def run_merge(groups: list[FunctionGroup], model: str, base_url: str) -> MergeResult:
    """
    One LLM call per FunctionGroup. The LLM returns 1+ rewritten requirements
    per function. Progress is printed to stdout.
    """
    # Load verification methods context once for the whole run
    print(f"  Prompt log → {log_file_path()}")
    vm_context = verification_methods_block()
    rt_context = requirement_type_block()

    n_groups    = len(groups)
    hlrs: list[HighLevelRequirement] = []
    hlr_counter = 0

    for i, group in enumerate(groups, start=1):
        if not group.draft.strip():
            print(f"  [{i}/{n_groups}] SKIP  '{group.function_name}' — empty draft")
            continue

        print(
            f"  [{i}/{n_groups}] Rewriting  '{group.function_name}'…",
            end=" ", flush=True,
        )

        id_start = f"#{hlr_counter + 1:03d}"
        raw_list = rewrite_function(group, id_start, model, base_url,
                                    vm_context=vm_context,
                                    rt_context=rt_context) 
        for raw in raw_list:
            hlr_counter += 1
            notes = raw.get("compliance_notes", "")
            hlrs.append(HighLevelRequirement(
                id                      = f"LLR-{hlr_counter:03d}",
                function_name           = group.function_name,
                name                    = str(raw.get("name", group.function_name)),
                description             = str(raw.get("description", group.draft)),
                verification_method     = str(raw.get("verification_method", "Test")),
                requirement_type        = str(raw.get("requirement_type", "Functional")),
                engineering_discipline  = str(raw.get("engineering_discipline", "Software")),
            ))

        print(f"→ {len(raw_list)} requirement(s)")

    return MergeResult(
        hlrs           = hlrs,
        function_count = n_groups,
        llr_count      = n_groups,
        _groups        = groups,
    )
