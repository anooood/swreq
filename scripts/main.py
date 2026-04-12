#!/usr/bin/env python3
"""
main.py — LLR → Jama HLR CLI
------------------------------
Input CSV columns: Function Name, Requirements
Existing informal LLRs → LLM rewrites each one to be Jama-compliant
(single "shall" sentence, verifiable, unambiguous, technology-agnostic).

Usage:
    python main.py --input llrs.csv
    python main.py --input llrs.csv --output MyProject.xlsx
    python main.py --input llrs.csv --dry-run
"""

import sys as _sys
from pathlib import Path as _Path
_REPO_ROOT = _Path(__file__).resolve().parent if _Path(__file__).resolve().parent.name != "scripts" else _Path(__file__).resolve().parent.parent
if str(_REPO_ROOT) not in _sys.path:
    _sys.path.insert(0, str(_REPO_ROOT))

import argparse
import logging
import sys
import os
from pathlib import Path


logging.basicConfig(level=logging.WARNING, format="%(levelname)s  %(message)s")


# ---------------------------------------------------------------------------
# Argument parser
# ---------------------------------------------------------------------------

def build_parser() -> argparse.ArgumentParser:
    p = argparse.ArgumentParser(
        prog="llr-to-jama",
        description="Convert software requirements into Jama-ready HLRs (Excel).",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Modes:
  synthesise (default)  CSV columns: Function, Requirements
                        LLRs → LLM groups and writes HLRs from scratch

  rewrite               CSV columns: Function, HLR_Name, HLR_Draft
                        Existing draft HLRs → LLM rewrites to Jama standard

Examples:
  python main.py --input llrs.csv
  python main.py --input hlr_drafts.csv --mode rewrite
  python main.py --input llrs.csv --output MyProject.xlsx --model llama3.2
  python main.py --input llrs.csv --ollama-url http://192.168.1.10:11434
  python main.py --input llrs.csv --dry-run
        """,
    )
    p.add_argument("--input",  "-i", required=True, metavar="FILE",
                   help="Path to input CSV file.")
    p.add_argument("--output", "-o", default=None,  metavar="FILE",
                   help="Output Excel path. Defaults to <input_stem>_jama.xlsx.")
    p.add_argument("--model",  "-m", default="mistral:7b", metavar="MODEL",
                   help="Ollama model tag. Default: mistral:7b")
    p.add_argument("--ollama-url", default="http://localhost:11434", metavar="URL",
                   help="Ollama server base URL. Default: http://localhost:11434")
    p.add_argument("--dry-run", action="store_true",
                   help="Parse CSV and print summary, then exit without calling Ollama.")
    p.add_argument("--verbose", "-v", action="store_true",
                   help="Enable verbose logging.")
    return p


def resolve_output(input_path: str, output_arg: str | None) -> str:
    if output_arg:
        return output_arg
    stem   = Path(input_path).stem
    parent = Path(input_path).parent
    return str(parent / f"{stem}_jama.xlsx")


def check_ollama(base_url: str, model: str) -> bool:
    import requests
    try:
        resp = requests.get(f"{base_url.rstrip('/')}/api/tags", timeout=5)
        resp.raise_for_status()
        available = [m["name"] for m in resp.json().get("models", [])]
        if model not in available:
            print(f"\n⚠  Model '{model}' not found in Ollama.")
            print(f"   Available: {', '.join(available) or 'none'}")
            print(f"   Run:  ollama pull {model}\n")
            return False
        return True
    except Exception as exc:
        print(f"\n✗  Cannot reach Ollama at {base_url}: {exc}")
        print("   Make sure Ollama is running:  ollama serve\n")
        return False


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main():
    parser = build_parser()
    args   = parser.parse_args()

    if args.verbose:
        logging.getLogger().setLevel(logging.DEBUG)

    if not os.path.isfile(args.input):
        print(f"✗  Input file not found: {args.input}")
        sys.exit(1)

    # ── Select mode module ──────────────────────────────────────────────────
    from src.pipelines import inference_pipeline as merger_mod
    mode_label = "rewrite  (draft LLRs → Jama-compliant)"
    item_label = "draft LLR(s)"

    print(f"\n📄 Input    {args.input}")
    print(f"   Mode     {mode_label}\n")

    # ── Parse CSV ───────────────────────────────────────────────────────────
    try:
        groups = merger_mod.parse_groups_from_csv(args.input)
    except ValueError as exc:
        print(f"✗  CSV error: {exc}")
        sys.exit(1)

    total_items = sum(
        len(g.drafts) if hasattr(g, "drafts") else (1 if hasattr(g, "draft") else len(g.requirements))
        for g in groups
    )
    print(f"   {len(groups)} function group(s)  ·  {total_items} {item_label} total\n")

    for g in groups:
        count = len(g.drafts) if hasattr(g, "drafts") else (1 if hasattr(g, "draft") else len(g.requirements))
        print(f"   ▸  {g.function_name}  ({count} {item_label})")
    print()

    if args.dry_run:
        print("ℹ  Dry run — exiting before LLM call.")
        sys.exit(0)

    # ── Check Ollama ────────────────────────────────────────────────────────
    print(f"🤖 Ollama   {args.ollama_url}  /  model: {args.model}\n")
    if not check_ollama(args.ollama_url, args.model):
        sys.exit(1)

    # ── Run ─────────────────────────────────────────────────────────────────
    # label = "Rewriting HLRs" if args.mode == "rewrite" else "Synthesising HLRs"
    # print(f"⚙  {label}…\n")
    result = merger_mod.run_merge(groups, model=args.model, base_url=args.ollama_url)

    print(f"\n✔  {len(result.hlrs)} HLR(s) produced across "
          f"{result.function_count} function(s)\n")

    # ── Export ──────────────────────────────────────────────────────────────
    from src.pipelines.exporter import export_to_excel
    output_path = resolve_output(args.input, args.output)
    print(f"📊 Writing  {output_path}")
    try:
        export_to_excel(result, output_path, filename=Path(args.input).stem)
    except Exception as exc:
        print(f"✗  Excel export failed: {exc}")
        sys.exit(1)

    print(f"✔  Done — {output_path}\n")

    # ── Summary table ───────────────────────────────────────────────────────
    from collections import defaultdict
    by_func: dict[str, list] = defaultdict(list)
    for hlr in result.hlrs:
        by_func[hlr.function_name].append(hlr)

    col_w = max(len(f) for f in by_func) + 2
    print(f"  {'Function':<{col_w}}  HLRs  IDs")
    print(f"  {'-'*col_w}  ----  ---")
    for func, hlrs in by_func.items():
        ids = ", ".join(h.id for h in hlrs)
        print(f"  {func:<{col_w}}  {len(hlrs):<4}  {ids}")
    print()


if __name__ == "__main__":
    main()