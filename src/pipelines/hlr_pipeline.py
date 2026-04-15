"""
hlr_pipeline.py
---------------
Stage 3: Jama-compliant LLR → High-Level Requirements (HLR).

Takes the Jama LLR CSV produced by Stage 2 and consolidates related
low-level requirements into fewer, higher-level requirements suitable
for the top of a Jama requirement hierarchy.

CSV columns (case-sensitive):
  "Function Name"  — name of the source function
  "Requirements"   — Jama LLR text (shall statements from Stage 2)

One LLM call per function row. The LLM:
  - Reads all LLRs for a function and identifies related groups
  - Merges related LLRs into consolidated HIGH-LEVEL requirements
  - Preserves ALL numerical values from the original LLRs
  - Produces significantly fewer requirements (typically 3-8 per function)

Compatible with exporter.py (same MergeResult / HighLevelRequirement shape).
"""

from __future__ import annotations

import json
import logging
from dataclasses import dataclass, field
from typing import Any

import pandas as pd

from src.utils.reference_loader import verification_methods_block

logger = logging.getLogger(__name__)

CSV_FUNCTION_COL = "Function Name"
CSV_DRAFT_COL    = "Requirements"

# ---------------------------------------------------------------------------
# Data classes  (same shape as inference_pipeline.py so exporter.py works)
# ---------------------------------------------------------------------------

@dataclass
class FunctionGroup:
    """One function and its combined LLR string."""
    function_name: str
    draft: str


@dataclass
class HighLevelRequirement:
    id: str
    function_name: str
    name: str
    description: str
    verification_method: str
    requirement_type: str


@dataclass
class MergeResult:
    hlrs: list[HighLevelRequirement]
    function_count: int = 0
    llr_count: int = 0
    _groups: list[FunctionGroup] = field(default_factory=list)


# ---------------------------------------------------------------------------
# CSV parser
# ---------------------------------------------------------------------------

def parse_groups_from_csv(path: str) -> list[FunctionGroup]:
    """
    Read CSV with columns 'Function Name' and 'Requirements'.
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

    return [
        FunctionGroup(function_name=f, draft=" ".join(parts))
        for f, parts in groups.items()
    ]


# ---------------------------------------------------------------------------
# LLM prompt — HLR synthesis (merging)
# ---------------------------------------------------------------------------

_CLEANUP_PROMPT = """\
You are a systems-engineering expert reviewing low-level software requirements.

You are given a JSON array of LLRs for a software function.

YOUR TASK:
Remove LLRs that are trivial implementation details with no behavioral significance, such as:
- Formatting or constructing auxiliary strings
- Setting a flag to 0 or 1
- Clearing or zeroing a structure or buffer
- Incrementing or decrementing a counter
- Any other bookkeeping step that does not describe observable system behavior

RULES:
- Do NOT rephrase, reword, or modify any retained LLR — return its JSON object exactly as received.
- Every numerical value in retained LLRs must be preserved as-is.

Respond ONLY with a JSON array of the retained LLR objects. No text outside the JSON array.

LLRs : {llrs_json}
"""

# ---------------------------------------------------------------------------
# JSON parser
# ---------------------------------------------------------------------------

def _safe_json_parse(raw: str) -> Any:
    text = raw.strip()
    if text.startswith("```"):
        lines = text.splitlines()
        end   = -1 if lines[-1].strip() == "```" else len(lines)
        text  = "\n".join(lines[1:end])
    return json.loads(text)


# ---------------------------------------------------------------------------
# Public helpers (used by app.py endpoint)
# ---------------------------------------------------------------------------

def get_hlr_prompt() -> str:
    """Return the HLR prompt template string."""
    return _CLEANUP_PROMPT