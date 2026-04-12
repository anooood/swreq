"""
notebooks/llm_streamlit_app.py
-------------------------------
Unified Streamlit UI for the two-stage requirements pipeline:

  Stage 1 — Code Analysis  : Parse C source → generate Low-Level Requirements (LLR)
  Stage 2 — Jama Compliance : Rewrite LLRs into Jama-compliant LLRs

Outputs are saved to artifacts/ (Word .docx, CSV, Jama Excel .xlsx).
All LLM calls go through the FastAPI server at http://127.0.0.1:8000,
which forwards them to a local Ollama instance.
"""

import ast as asast
import json
import sys
import time
from datetime import datetime
from pathlib import Path

# ---------------------------------------------------------------------------
# Ensure the repo root is on sys.path so `src.*` imports resolve correctly
# regardless of which directory Streamlit is launched from.
# ---------------------------------------------------------------------------
_REPO_ROOT = Path(__file__).resolve().parent.parent
if str(_REPO_ROOT) not in sys.path:
    sys.path.insert(0, str(_REPO_ROOT))

import glob
import pandas as pd
import regex as re
import requests
import streamlit as st
from pycparser import c_generator, parse_file

from src.dataset.preprocess import (
    ContextExtractor,
    add_comments,
    clean_header_comments,
    extract_function_context,
    extract_header_comments,
    generate_functions,
    get_comment_positions,
    get_header_function_descriptions,
    get_identifier_mapping,
    pre_processing,
)
from src.utils.prompt_templates import (
    identifier_rewrite_prompt,
    name_prompt_template,
    reqs_prompt_template,
    second_prompt,
)
from src.utils.word_output import generate_doc_from_cleaned_and_df

# ─────────────────────────────────────────────────────────────────────────────
# Config
# ─────────────────────────────────────────────────────────────────────────────

API_URL        = "http://127.0.0.1:8000"
GIT_FOLDER     = "data/P3_MCP_Application/cmake-src/src"
ARTIFACTS_DIR  = Path("artifacts")
ARTIFACTS_DIR.mkdir(parents=True, exist_ok=True)

# ─────────────────────────────────────────────────────────────────────────────
# Page config
# ─────────────────────────────────────────────────────────────────────────────

st.set_page_config(
    page_title="Software Requirements Generator",
    layout="wide",
)

# ─────────────────────────────────────────────────────────────────────────────
# Active model config — read from server at startup (display only)
# ─────────────────────────────────────────────────────────────────────────────

try:
    _cfg_resp    = requests.get(f"{API_URL}/config", timeout=3)
    _active_cfg  = _cfg_resp.json() if _cfg_resp.ok else {}
except Exception:
    _active_cfg  = {}

STAGE1_MODEL = _active_cfg.get("stage1_model", "mistral:7b")
STAGE2_MODEL = _active_cfg.get("stage2_model", "mistral:7b")


# ─────────────────────────────────────────────────────────────────────────────
# Helpers
# ─────────────────────────────────────────────────────────────────────────────

def strip_markdown(text: str) -> str:
    if not text:
        return ""
    text = text.replace('"', "")
    text = re.sub(r"```.*?```", "", text, flags=re.DOTALL)
    text = re.sub(r"`([^`]*)`", r"\1", text)
    text = re.sub(r"\*\*(.*?)\*\*", r"\1", text)
    text = re.sub(r"\*(.*?)\*", r"\1", text)
    text = re.sub(r"#+\s*", "", text)
    text = re.sub(r"[-•]\s+", "", text)
    text = re.sub(r"\n{2,}", "\n", text)
    return text.strip()


def auto_height(text, min_lines=4, max_lines=30, line_px=22):
    lines = max(text.count("\n") + 1, min_lines)
    return (min(lines, max_lines) * line_px) + 100


def format_globals(globals_dict):
    if not globals_dict:
        return "_No global variables referenced._"
    lines = ["**Global variables used:**"]
    for k, v in globals_dict.items():
        lines.append(f"- `{k}` = `{v}`")
    return "\n".join(lines)


IDENTIFIER = re.compile(r"[A-Za-z_][A-Za-z0-9_.]*")


def replace_exact_identifiers(text, mapping):
    return IDENTIFIER.sub(lambda m: mapping.get(m.group(0), m.group(0)), text)


def remove_consecutive_duplicates(text):
    return re.sub(r"\b(\w+)(?:\s+\1\b)+", r"\1", text, flags=re.IGNORECASE)


def on_module_change():
    st.session_state["requirements"].clear()
    st.session_state["functions"] = []
    st.session_state["cleaned_headers"] = None
    st.session_state["generation_started"] = False
    st.session_state["generation_completed"] = False
    st.session_state["jama_result"] = None
    for k in list(st.session_state.keys()):
        if k.startswith("title_") or k.startswith("body_"):
            del st.session_state[k]


def _artifacts_path(filename: str) -> Path:
    """Return an artifacts/ path with a datestamp prefix."""
    stamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    return ARTIFACTS_DIR / f"{stamp}_{filename}"


def append_df_to_excel(file_path: Path, df: pd.DataFrame, sheet_name: str = "Sheet1"):
    if not file_path.exists():
        df.to_excel(file_path, sheet_name=sheet_name, index=False)
        return
    with pd.ExcelWriter(file_path, engine="openpyxl", mode="a",
                        if_sheet_exists="overlay") as writer:
        start_row = writer.book[sheet_name].max_row if sheet_name in writer.book.sheetnames else 0
        df.to_excel(writer, sheet_name=sheet_name, startrow=start_row,
                    index=False, header=(start_row == 0))


# ─────────────────────────────────────────────────────────────────────────────
# Session state init
# ─────────────────────────────────────────────────────────────────────────────

for _key, _default in [
    ("generation_started", False),
    ("generation_completed", False),
    ("functions", []),
    ("requirements", {}),
    ("jama_result", None),
    ("inference_times", []),
    ("variables_leaked_all", []),
    ("globals_missing_all", []),
    ("cleaned_headers", None),
]:
    if _key not in st.session_state:
        st.session_state[_key] = _default

# ─────────────────────────────────────────────────────────────────────────────
# Main title + tabs
# ─────────────────────────────────────────────────────────────────────────────

st.title("🔧 Software Requirements Generator")
st.caption("Stage 1: C code → LLR  |  Stage 2: LLR → Jama-compliant LLR")

tab1, tab2 = st.tabs(["📄 Stage 1 — Code → LLR", "📋 Stage 2 — LLR → Jama LLR"])


# ═════════════════════════════════════════════════════════════════════════════
#  STAGE 1 — Code Analysis → LLR
# ═════════════════════════════════════════════════════════════════════════════

with tab1:
    st.header("Stage 1: C Code → Low-Level Requirements")
    st.markdown(
        "Select a C module from your codebase. The tool parses the source, "
        "extracts each function, and generates detailed software requirements "
        "using the LLM."
    )

    # ── API health + active model info ───────────────────────────────────
    try:
        requests.get(f"{API_URL}/health", timeout=2)
        st.caption(f"API ✅  Ollama ✅  — model: `{STAGE1_MODEL}` — outputs saved to `artifacts/`")
    except Exception:
        st.error("API ❌  — start the server with `bash run.sh`")

    st.divider()

    # Module selector
    modules = sorted(Path(GIT_FOLDER).glob("*.c"))
    if not modules:
        st.warning(f"No `.c` files found in `{GIT_FOLDER}`. "
                   "Add your codebase to `data/P3_MCP_Application/cmake-src/src/`.")
        st.stop()

    modules_names = [m.name.replace(".c", "") for m in modules]
    selected_module = st.selectbox(
        "Select a C module:",
        [m.name for m in modules],
        on_change=on_module_change,
    )

    with open(Path(GIT_FOLDER) / selected_module, encoding="utf-8", errors="ignore") as f:
        source_preview = f.read()
    with st.expander("📂 Source preview", expanded=False):
        st.code(source_preview[:3000] + ("..." if len(source_preview) > 3000 else ""),
                language="c")

    # Generate button
    if st.button("▶ Generate LLRs", type="primary"):
        st.session_state.update({
            "requirements": {},
            "functions": [],
            "generation_started": True,
            "generation_completed": False,
            "variables_leaked_all": [],
            "globals_missing_all": [],
            "inference_times": [],
            "jama_result": None,
        })

    functions = st.session_state.get("functions", [])

    if st.session_state["generation_started"] and not st.session_state["generation_completed"]:
        with st.spinner("Parsing C source and generating requirements…"):
            output_file, input_file = pre_processing(selected_module)
            functions = generate_functions(output_file, input_file)
            st.session_state["functions"] = functions

            cleaned_headers = clean_header_comments(extract_header_comments(input_file))
            st.session_state["cleaned_headers"] = cleaned_headers

            ast = parse_file(output_file)
            extractor = ContextExtractor()
            extractor.visit(ast)
            generator_ast = c_generator.CGenerator()
            function_context = extract_function_context(extractor)

            st.subheader(f"Requirements for **{selected_module}**")
            progress = st.progress(0)

            for i, fn in enumerate(functions):
                placeholder = st.empty()

                if i not in st.session_state["requirements"]:
                    start_time = time.time()

                    orig_src   = fn["content"]
                    func_node  = extractor.function_defs.get(fn["name"])
                    emb_src    = generator_ast.visit(func_node)
                    mapping    = get_identifier_mapping(orig_src, emb_src)

                    name_prompt = name_prompt_template.format(
                        header_comments=st.session_state["cleaned_headers"],
                        function_name=fn["name"],
                    )
                    reqs_prompt = reqs_prompt_template.format(
                        C_FUNCTION=fn["content"],
                        GLOBAL_VARIABLES=mapping,
                        header_comments = st.session_state["cleaned_headers"]
                    )

                    response = requests.post(
                        f"{API_URL}/generate",
                        json={"prompts": [name_prompt, reqs_prompt]},
                        timeout=300,
                    )

                    if response.status_code != 200:
                        placeholder.error(f"Model failed for {fn['name']}: {response.text}")
                        continue

                    data      = response.json()
                    fixed_req = data["requirements"][1]

                    for k, v in mapping.items():
                        cleaned_v = re.sub(r"\s*\([^)]*\)", "", v)
                        fixed_req = fixed_req.replace(k, cleaned_v)

                    # --- Leaked syntax detection ---
                    # ctx = function_context[fn["name"]]
                    # leaked = []
                    # for var in ctx["globals_used"] + ctx["locals_declared"]:
                    #     if var not in modules_names and var in fixed_req:
                    #         leaked.append(var)
                    # for func in ctx["functions_called"]:
                    #     if func in fixed_req:
                    #         leaked.append(func)
                    # for struct in ctx["used_structures"]:
                    #     if struct not in modules_names and struct in fixed_req:
                    #         leaked.append(struct)
                    # if fn["name"] in fixed_req:
                    #     leaked.append(fn["name"])

                    # if leaked:
                    #     post_prompt = identifier_rewrite_prompt.format(
                    #         REQUIREMENTS=fixed_req,
                    #         VARIABLES=leaked,
                    #     )
                    #     r2   = requests.post(
                    #         f"{API_URL}/generate",
                    #         json={"prompts": ["N/A", post_prompt]},
                    #         timeout=300,
                    #     )
                    #     d2   = r2.json()
                    #     repl = asast.literal_eval(d2["requirements"][1])
                    #     vmap = dict(zip(leaked, repl))
                    #     final_req = remove_consecutive_duplicates(
                    #         replace_exact_identifiers(fixed_req, vmap)
                    #     )
                    # else:
                    #     final_req = fixed_req

                    final_req = fixed_req
                    elapsed = time.time() - start_time
                    st.session_state["inference_times"].append(elapsed)

                    # Track quality metrics
                    # for var in ctx["globals_used"] + ctx["locals_declared"]:
                    #     if var not in modules_names and var in final_req:
                    #         st.session_state["variables_leaked_all"].append(var)
                    for k, v in mapping.items():
                        cleaned_v = re.sub(r"\s*\([^)]*\)", "", v).replace(")", "").strip()
                        if cleaned_v not in final_req:
                            st.session_state["globals_missing_all"].append(cleaned_v)

                    st.session_state["requirements"][i] = {
                        "Function":            strip_markdown(data["requirements"][0]),
                        "Requirements":        final_req.strip(),
                        "ModelResponseTimeSec": elapsed,
                        "Applied":             False,
                        "Globals":             mapping,
                    }

                # Render editable form
                req = st.session_state["requirements"][i]
                col_req, col_code = placeholder.columns([1, 1])

                with col_req:
                    form_key  = f"{selected_module}_form_{i}"
                    title_key = f"title_{selected_module}_{i}"
                    body_key  = f"body_{selected_module}_{i}"

                    with st.form(form_key, clear_on_submit=False):
                        title = st.text_input("Requirement Title", value=req["Function"],   key=title_key)
                        body  = st.text_area("Requirement Text",  value=req["Requirements"],
                                             key=body_key, height=auto_height(req["Requirements"]))
                        if st.form_submit_button("Apply changes"):
                            st.session_state["requirements"][i].update({
                                "Function":     st.session_state[title_key],
                                "Requirements": st.session_state[body_key],
                                "Applied":      True,
                            })
                            st.success("Changes applied")

                    st.markdown(format_globals(req["Globals"]))
                    st.markdown(f"**Response time:** {round(req['ModelResponseTimeSec'], 2)} s")

                with col_code:
                    with st.expander("Code Reference", expanded=True):
                        st.code(fn["content"], language="c")

                st.divider()
                progress.progress((i + 1) / len(functions))

        st.session_state["generation_completed"] = True

    # ── Export Section ────────────────────────────────────────────────────────

    if st.session_state["requirements"]:
        st.subheader("📤 Export Stage 1 Outputs")
        total_time = sum(st.session_state["inference_times"])
        st.markdown(f"**Total inference time:** {round(total_time / 60, 2)} min")

        df_llr = pd.DataFrame.from_dict(st.session_state["requirements"], orient="index")

        col_word, col_csv = st.columns(2)

        with col_word:
            if st.button("📝 Save Word Document"):
                module_stem = selected_module.replace(".c", "")
                docx_path   = _artifacts_path(f"{module_stem}_LLR.docx")

                # Log run metadata
                log_row = {
                    "Module":          selected_module,
                    "Leaked Syntax":   json.dumps(list(set(st.session_state["variables_leaked_all"]))),
                    "Missing Globals": json.dumps(list(set(st.session_state["globals_missing_all"]))),
                }
                log_path = ARTIFACTS_DIR / "run_log.xlsx"
                append_df_to_excel(log_path, pd.DataFrame([log_row]))

                generate_doc_from_cleaned_and_df(
                    df=df_llr,
                    cleaned=st.session_state["cleaned_headers"],
                    out_path=str(docx_path),
                    debug=True,
                )
                st.success(f"Saved → `{docx_path}`")
                st.download_button(
                    label="⬇ Download Word (.docx)",
                    data=docx_path.read_bytes(),
                    file_name=docx_path.name,
                    mime="application/vnd.openxmlformats-officedocument.wordprocessingml.document",
                )

        with col_csv:
            if st.button("📊 Save CSV"):
                module_stem = selected_module.replace(".c", "")
                csv_path    = _artifacts_path(f"{module_stem}_LLR.csv")

                rows = [
                    {"Function Name": r["Function"], "Requirements": r["Requirements"]}
                    for r in st.session_state["requirements"].values()
                ]
                df_csv = pd.DataFrame(rows, columns=["Function Name", "Requirements"])
                df_csv.to_csv(csv_path, index=False)
                st.success(f"Saved → `{csv_path}`")
                st.download_button(
                    label="⬇ Download CSV",
                    data=csv_path.read_bytes(),
                    file_name=csv_path.name,
                    mime="text/csv",
                )

        st.info("➡ Use the **Stage 2** tab to convert these LLRs into Jama-compliant LLRs.")
    else:
        st.info("No requirements yet. Select a module and click **Generate LLRs**.")


# ═════════════════════════════════════════════════════════════════════════════
#  STAGE 2 — LLR → Jama LLR
# ═════════════════════════════════════════════════════════════════════════════

with tab2:
    st.header("Stage 2: LLR → Jama-Compliant Low-Level Requirements")
    st.markdown(
        "Upload a CSV of LLRs (columns: `Function Name`, `Requirements`) "
        "or use the output from Stage 1. The LLM rewrites each draft into a "
        "formal Jama-compliant **shall** statement."
    )

    st.caption(f"Model: `{STAGE2_MODEL}` — configured in `configs/model.yaml`")
    st.divider()

    # Input source selector
    input_source = st.radio(
        "LLR input source",
        ["Use Stage 1 output (current session)", "Upload CSV file"],
        horizontal=True,
    )

    groups_df: pd.DataFrame | None = None

    if input_source == "Use Stage 1 output (current session)":
        if st.session_state["requirements"]:
            rows = [
                {"Function Name": r["Function"], "Requirements": r["Requirements"]}
                for r in st.session_state["requirements"].values()
            ]
            groups_df = pd.DataFrame(rows)
            st.success(f"{len(groups_df)} function(s) loaded from Stage 1.")
            st.dataframe(groups_df, use_container_width=True)
        else:
            st.warning("No Stage 1 output yet — run Stage 1 first, or upload a CSV.")

    else:
        uploaded = st.file_uploader("Upload LLR CSV", type=["csv"])
        if uploaded:
            groups_df = pd.read_csv(uploaded, dtype=str).fillna("")
            missing = [c for c in ("Function Name", "Requirements") if c not in groups_df.columns]
            if missing:
                st.error(f"CSV is missing columns: {missing}")
                groups_df = None
            else:
                st.success(f"{len(groups_df)} row(s) loaded.")
                st.dataframe(groups_df, use_container_width=True)

    # Run Stage 2
    if groups_df is not None and st.button("▶ Generate Jama LLRs", type="primary"):
        from src.pipelines.inference_pipeline import (
            FunctionGroup,
            MergeResult,
            HighLevelRequirement,
            run_merge,
        )

        # Build FunctionGroup list
        groups: dict[str, list[str]] = {}
        for _, row in groups_df.iterrows():
            fn   = str(row["Function Name"]).strip()
            req  = str(row["Requirements"]).strip()
            if fn and req:
                groups.setdefault(fn, []).append(req)

        function_groups = [
            FunctionGroup(function_name=fn, draft=" ".join(parts))
            for fn, parts in groups.items()
        ]

        progress_bar = st.progress(0)
        status_box   = st.empty()
        hlrs_all: list[HighLevelRequirement] = []
        hlr_counter = 0

        for idx, group in enumerate(function_groups):
            status_box.info(f"Rewriting [{idx+1}/{len(function_groups)}]: **{group.function_name}**…")

            resp = requests.post(
                f"{API_URL}/rewrite",
                json={
                    "function_name": group.function_name,
                    "draft":         group.draft,
                },
                timeout=300,
            )

            if resp.status_code != 200:
                st.warning(f"Rewrite failed for {group.function_name}: {resp.text}")
                continue

            raw_list = resp.json().get("requirements", [])
            for raw in raw_list:
                hlr_counter += 1
                hlrs_all.append(HighLevelRequirement(
                    id                  = f"LLR-{hlr_counter:03d}",
                    function_name       = group.function_name,
                    name                = str(raw.get("name", group.function_name)),
                    description         = str(raw.get("description", group.draft)),
                    verification_method = str(raw.get("verification_method", "Analysis")),
                    requirement_type    = str(raw.get("requirement_type", "Functional")),
                ))

            progress_bar.progress((idx + 1) / len(function_groups))

        status_box.success(f"✅ {len(hlrs_all)} LLR(s) generated from {len(function_groups)} function(s).")

        result = MergeResult(
            hlrs           = hlrs_all,
            function_count = len(function_groups),
            llr_count      = len(groups_df),
            _groups        = function_groups,
        )
        st.session_state["jama_result"] = result

    # ── Display + Export Stage 2 ──────────────────────────────────────────────

    if st.session_state.get("jama_result"):
        result = st.session_state["jama_result"]
        st.subheader(f"Generated LLRs ({len(result.hlrs)} total)")

        # Preview table
        preview_rows = [
            {
                "ID":                  h.id,
                "Function":            h.function_name,
                "Name":                h.name,
                "Description":         h.description[:120] + ("…" if len(h.description) > 120 else ""),
                "Verification":        h.verification_method,
                "Type":                h.requirement_type,
            }
            for h in result.hlrs
        ]
        st.dataframe(pd.DataFrame(preview_rows), use_container_width=True)

        st.subheader("📤 Export Stage 2 Outputs")
        col_xlsx, col_csv2 = st.columns(2)

        with col_xlsx:
            if st.button("📊 Save Jama Excel (.xlsx)"):
                from src.pipelines.exporter import export_to_excel

                stem      = groups_df["Function Name"].iloc[0].split()[0] if groups_df is not None else "output"
                xlsx_path = _artifacts_path(f"{stem}_Jama_LLR.xlsx")
                export_to_excel(result, str(xlsx_path), filename=stem)
                st.success(f"Saved → `{xlsx_path}`")
                st.download_button(
                    label="⬇ Download Jama Excel",
                    data=xlsx_path.read_bytes(),
                    file_name=xlsx_path.name,
                    mime="application/vnd.openxmlformats-officedocument.spreadsheetml.sheet",
                )

        with col_csv2:
            if st.button("📄 Save Jama CSV"):
                stem     = groups_df["Function Name"].iloc[0].split()[0] if groups_df is not None else "output"
                csv_path = _artifacts_path(f"{stem}_Jama_LLR.csv")

                rows = [
                    {
                        "ID":                  h.id,
                        "Function Name":       h.function_name,
                        "Name":                h.name,
                        "Description":         h.description,
                        "Verification Method": h.verification_method,
                        "Requirement Type":    h.requirement_type,
                    }
                    for h in result.hlrs
                ]
                pd.DataFrame(rows).to_csv(csv_path, index=False)
                st.success(f"Saved → `{csv_path}`")
                st.download_button(
                    label="⬇ Download Jama CSV",
                    data=csv_path.read_bytes(),
                    file_name=csv_path.name,
                    mime="text/csv",
                )
    elif groups_df is not None:
        st.info("Click **Generate Jama LLRs** to start Stage 2.")
    else:
        st.info("Provide LLR input above to begin.")
