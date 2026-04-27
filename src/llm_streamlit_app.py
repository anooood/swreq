"""
src/llm_streamlit_app.py
-------------------------
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
import os
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
    trivial_filter_prompt,
)
from src.utils.word_output import generate_doc_from_cleaned_and_df

from core.auth import require_login, sidebar_user_info

# ─────────────────────────────────────────────────────────────────────────────
# Page config — must be the very first Streamlit call
# ─────────────────────────────────────────────────────────────────────────────

st.set_page_config(
    page_title="Software Requirements Generator",
    page_icon="🤖",
    layout="wide",
    initial_sidebar_state="expanded",
)

# Login gate — populates st.session_state["api_token"]
_user = require_login()

# User info + logout at the top of the sidebar
sidebar_user_info()

# ─────────────────────────────────────────────────────────────────────────────
# Config
# ─────────────────────────────────────────────────────────────────────────────

API_URL = os.getenv("API_URL", "http://127.0.0.1:8000")

def _api_headers() -> dict:
    return {"Authorization": f"Bearer {st.session_state.get('api_token', '')}"}

GIT_FOLDER     = ""  # set dynamically via upload below
ARTIFACTS_DIR  = Path("artifacts")
ARTIFACTS_DIR.mkdir(parents=True, exist_ok=True)

# ─────────────────────────────────────────────────────────────────────────────
# Active model config — read from server at startup (display only)
# ─────────────────────────────────────────────────────────────────────────────

try:
    _cfg_resp    = requests.get(f"{API_URL}/config", timeout=3, headers=_api_headers())
    _active_cfg  = _cfg_resp.json() if _cfg_resp.ok else {}
except Exception:
    _active_cfg  = {}

STAGE1_MODEL = _active_cfg.get("stage1_model", "qwen3-coder:30b")
STAGE2_MODEL = _active_cfg.get("stage2_model", "qwen2.5:latest")

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
    for identifier, replacement in mapping.items():
        pattern = r'(?<!\S)' + re.escape(identifier) + r'(?!\S)'
        text = re.sub(pattern, replacement, text)
    return text


def remove_consecutive_duplicates(text):
    return re.sub(r"\b(\w+)(?:\s+\1\b)+", r"\1", text, flags=re.IGNORECASE)


def on_module_change():
    st.session_state["requirements"].clear()
    st.session_state["functions"] = []
    st.session_state["cleaned_headers"] = None
    st.session_state["generation_started"] = False
    st.session_state["generation_completed"] = False
    st.session_state["jama_result"] = None
    st.session_state["hlr_result"] = None
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

TRIVIAL_RULES = [
    # format/append/construct + auxiliary string
    r'\b(format|append|construct)\b.*\b(auxiliary string|aux string|string)\b',
    # auxiliary control
    r'\bauxiliary control\b',
    # flag + 0/1/True/False
    r'\bflag\b.*\b(0|1|true|false)\b',
    # clear + flag/string/structure
    r'\bclear\b.*\b(flag|string|structure)\b',
    # initialize + 0/zero/1/false/true
    r'\binitializ\w*\b.*\b(0|zero|1|false|true)\b'
]

def is_trivial(req: str) -> bool:
    return any(re.search(rule, req, re.IGNORECASE) for rule in TRIVIAL_RULES)

def filter_trivial_requirements(requirements: list[str]) -> list[str]:

    # Strip leading numbering (e.g. "1.", "10.")
    stripped = [re.sub(r'^\d+\.\s*', '', req.strip()) for req in requirements]
    kept = [req for req in stripped if req.strip() and not is_trivial(req)]

    # Re-number from 1
    return [f"{i}. {req}" for i, req in enumerate(kept, 1)]

# ─────────────────────────────────────────────────────────────────────────────
# Session state init
# ─────────────────────────────────────────────────────────────────────────────

for _key, _default in [
    ("generation_started", False),
    ("generation_completed", False),
    ("functions", []),
    ("requirements", {}),
    ("jama_result", None),
    ("hlr_result", None),
    ("inference_times", []),
    ("stage2_inference_times", []),
    ("variables_leaked_all", []),
    ("globals_missing_all", []),
    ("cleaned_headers", None),
    ("git_folder", GIT_FOLDER),
    ("selected_module", None),
]:
    if _key not in st.session_state:
        st.session_state[_key] = _default

# ═════════════════════════════════════════════════════════════════════════════
#  SIDEBAR — Codebase upload + module selection
# ═════════════════════════════════════════════════════════════════════════════

with st.sidebar:
    st.header("📁 Codebase")
    st.caption("Upload your C codebase (as a ZIP file), then pick a module to analyse.")

    _current_folder = st.session_state.get("git_folder", "")
    _folder_exists  = bool(_current_folder) and Path(_current_folder).exists()

    if _folder_exists:
        st.success("Codebase loaded")
        st.caption(f"`{_current_folder}`")
    else:
        st.info("No codebase loaded yet.")

    uploaded_zip = st.file_uploader(
        "Upload codebase ZIP",
        type=["zip"],
        key="codebase_zip",
        help="The folder containing the most .c files will be auto-detected.",
    )

    # Only process each upload once. Streamlit's file_uploader keeps the
    # file across reruns — without this guard, every button click on the
    # page (Apply changes, Save CSV, etc.) would re-extract the ZIP.
    _last_processed = st.session_state.get("_last_processed_zip_id")
    if uploaded_zip and uploaded_zip.file_id != _last_processed:
        import zipfile, tempfile
        from collections import Counter
        import shutil
        extract_root = Path("data") / "uploaded_codebase"
        # Clear any previous upload to avoid permission/leftover issues
        if extract_root.exists():
            shutil.rmtree(extract_root, ignore_errors=True)
        extract_root.mkdir(parents=True, exist_ok=True)

        with tempfile.TemporaryDirectory() as tmp:
            tmp_zip = Path(tmp) / uploaded_zip.name
            tmp_zip.write_bytes(uploaded_zip.read())
            with zipfile.ZipFile(tmp_zip, "r") as z:
                z.extractall(extract_root)

        # Auto-detect the folder with the most .c files
        c_files = list(extract_root.rglob("*.c"))
        if c_files:
            folder_counts   = Counter(str(f.parent) for f in c_files)
            detected_folder = folder_counts.most_common(1)[0][0]
            st.session_state["git_folder"]            = detected_folder
            st.session_state["_last_processed_zip_id"] = uploaded_zip.file_id
            on_module_change()
            st.success(f"Detected: `{detected_folder}`")
            st.rerun()
        else:
            st.session_state["_last_processed_zip_id"] = uploaded_zip.file_id
            st.error("No `.c` files found in the uploaded ZIP.")

    # Module selector — only shown once a codebase is loaded
    _git_folder = st.session_state.get("git_folder", "")
    if _git_folder and Path(_git_folder).exists():
        modules = sorted(Path(_git_folder).glob("*.c"))
        if modules:
            module_names = [m.name for m in modules]

            # Preserve previously-selected module across reruns
            prev = st.session_state.get("selected_module")
            default_idx = module_names.index(prev) if prev in module_names else 0

            selected_module = st.selectbox(
                "C module",
                module_names,
                index=default_idx,
                key="_sidebar_module_selector",
            )

            # If the user picked a different module, reset state
            if selected_module != st.session_state.get("selected_module"):
                st.session_state["selected_module"] = selected_module
                on_module_change()
        else:
            st.warning("No `.c` files found in the detected folder.")
            st.session_state["selected_module"] = None
    else:
        st.session_state["selected_module"] = None

# ─────────────────────────────────────────────────────────────────────────────
# Main title + tabs
# ─────────────────────────────────────────────────────────────────────────────

st.title("💻⚙️ Software Requirements Generator")
st.caption("Stage 1: C code → LLR  |  Stage 2: LLR → Jama LLR")

tab1, tab2 = st.tabs(["📄 Stage 1 — Code → LLR", "📋 Stage 2 — LLR → Jama LLR"])


# ═════════════════════════════════════════════════════════════════════════════
#  STAGE 1 — Code Analysis → LLR
# ═════════════════════════════════════════════════════════════════════════════

with tab1:
    st.header("Stage 1: C Code → Low-Level Requirements (LLRs)")
    st.markdown(
        "Pick a C module from the sidebar. The tool parses the source, "
        "extracts each function, and generates detailed software requirements "
        "using the LLM."
    )

    # ── API health + active model info ───────────────────────────────────
    try:
        requests.get(f"{API_URL}/health", timeout=2, headers=_api_headers())
        st.caption(f"LLM: `{STAGE1_MODEL}` — outputs saved to `artifacts/`")
    except Exception:
        st.error("API ❌  — start the server with `bash run.sh`")

    st.divider()

    # Gate the rest of Stage 1 on having a codebase + selected module
    _git_folder = st.session_state.get("git_folder", "")
    if not _git_folder or not Path(_git_folder).exists():
        st.info("⬅ Upload a codebase ZIP from the sidebar to get started.")
        st.stop()

    selected_module = st.session_state.get("selected_module")
    if not selected_module:
        st.warning("⬅ Select a C module from the sidebar.")
        st.stop()

    modules = sorted(Path(_git_folder).glob("*.c"))
    modules_names = [m.name.replace(".c", "") for m in modules]

    with open(Path(_git_folder) / selected_module, encoding="utf-8", errors="ignore") as f:
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

    _did_generate_this_run = False

    if st.session_state["generation_started"] and not st.session_state["generation_completed"]:
        _did_generate_this_run = True
        # Reset the Stage 1 model in Ollama so every batch starts from a
        # pristine KV-cache state. Without this, quality degrades on
        # subsequent runs against the same server process — cached prompt
        # prefixes from prior requests contaminate attention state for
        # later requests. The /reset_stage1 endpoint unloads and reloads
        # the Stage 1 model, giving each batch the same clean-start
        # behaviour that `./run.sh` provides on a fresh server start.
        try:
            requests.post(f"{API_URL}/reset_stage1", timeout=120, headers=_api_headers())
        except Exception as exc:
            st.warning(f"Stage 1 reset failed: {exc} — continuing anyway.")

        with st.spinner("Parsing C source and generating requirements…"):
            output_file, input_file = pre_processing(selected_module, git_folder=_git_folder)
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

                    orig_src  = fn["content"]
                    func_node = extractor.function_defs.get(fn["name"])
                    emb_src   = generator_ast.visit(func_node)
                    mapping   = get_identifier_mapping(orig_src, emb_src)

                    name_prompt = name_prompt_template.format(
                        header_comments=st.session_state["cleaned_headers"],
                        function_name=fn["name"],
                    )
                    reqs_prompt = reqs_prompt_template.format(
                        C_FUNCTION=fn["content"],
                        GLOBAL_VARIABLES=mapping,
                        header_comments=st.session_state["cleaned_headers"],
                    )

                    # ── Pass 1: Generate raw requirements from C code ──────────
                    response = requests.post(
                        f"{API_URL}/generate",
                        json={"prompts": [name_prompt, reqs_prompt]},
                        timeout=300,
                        headers=_api_headers())

                    if response.status_code != 200:
                        placeholder.error(f"Model failed for {fn['name']}: {response.text}")
                        continue

                    data      = response.json()
                    req_pass1 = data["requirements"][1]

                    # ── Pass 2: Remove trivial requirements ──────────────────
                    # Rule-based approach to remote trivial requirements

                    filtered = filter_trivial_requirements(req_pass1.split('\n'))
                    if not filtered:
                        req_pass2 = ""
                    else:
                        req_pass2 = "\n".join(filtered)

                    # ── Pass 3: Clean up leaked identifiers ───────────────────

                    # ── Leak detection ─────────────────────────────────────────
                    # Collect any C identifiers (variable names, function names,
                    # struct names, the function's own name) that leaked through
                    # into the requirements text despite the prompt instructions.

                    ctx = function_context[fn["name"]]
                    function_names = [x['name'] for x in functions]
                    leaked = []
                    for var in ctx["globals_used"] + ctx["locals_declared"]:
                        if var not in modules_names and var in req_pass2:
                            leaked.append(var)
                    for func in ctx["functions_called"]:
                        if func in req_pass2:
                            leaked.append(func)
                    for struct in ctx["used_structures"]:
                        if struct not in modules_names and struct in req_pass2:
                            leaked.append(struct)
                    for func_name in function_names:
                        if func_name in req_pass2:
                            leaked.append(func_name)
                    if fn["name"] in req_pass2:
                        leaked.append(fn["name"])

                    code_syntax_pattern = re.compile(
                        r'\b[A-Za-z_][A-Za-z0-9_]*(?:\.[A-Za-z_][A-Za-z0-9_]*)+\b'  # dot-joined
                        r'|'
                        r'\b[A-Za-z0-9]*_[A-Za-z0-9_]+\b'                             # underscore-joined
                        r'|'
                        r'\bs[A-Z][A-Za-z0-9]*(?:[A-Z][A-Za-z0-9]*)+\b'              # s-prefixed structs
                        r'|'
                        r'\b[A-Z][a-z0-9]+(?:[A-Z][A-Za-z0-9]*)+\b'                  # PascalCase — excludes all-caps
                        r'|'
                        r'\b[a-z]+(?:[A-Z][a-z0-9]+)+\b'                              # camelCase
                    )

                    for match in code_syntax_pattern.findall(req_pass2):
                        if (match not in leaked) and (match not in modules_names):
                            leaked.append(match)

                    leaked = list(dict.fromkeys(leaked))  # deduplicate, preserve order

                    if leaked:
                        post_prompt = identifier_rewrite_prompt.format(
                            REQUIREMENTS=req_pass2,
                            VARIABLES=leaked,
                        )
                        r2 = requests.post(
                            f"{API_URL}/generate",
                            json={"prompts": ["N/A", post_prompt]},
                            timeout=300,
                            headers=_api_headers()
                        )
                        if r2.status_code == 200:
                            raw_replacements = r2.json().get("requirements", ["", ""])[1]
                            try:
                                vmap = json.loads(raw_replacements)
                            except (json.JSONDecodeError, ValueError):
                                try:
                                    vmap = asast.literal_eval(raw_replacements)
                                except (ValueError, SyntaxError):
                                    vmap = {}

                            if isinstance(vmap, dict) and vmap:
                                final_req = replace_exact_identifiers(req_pass2, vmap)
                            else:
                                final_req = req_pass2
                        else:
                            final_req = req_pass2
                    else:
                        final_req = req_pass2

                    # ── Metrics tracking ──────────────────────────────────────
                    for var in ctx["globals_used"] + ctx["locals_declared"]:
                        if var not in modules_names and var in final_req:
                            st.session_state["variables_leaked_all"].append(var)

                    for k, v in mapping.items():
                        cleaned_v = re.sub(r"\s*\([^)]*\)", "", v).replace(")", "").strip()
                        if cleaned_v not in final_req:
                            st.session_state["globals_missing_all"].append(cleaned_v)

                    elapsed = time.time() - start_time
                    st.session_state["inference_times"].append(elapsed)

                    st.session_state["requirements"][i] = {
                        "Function":             strip_markdown(data["requirements"][0]),
                        "Requirements":         final_req.strip(),
                        "ModelResponseTimeSec": elapsed,
                        "Applied":              False,
                        "Approved":             False,
                        "Globals":              mapping,
                    }

                req = st.session_state["requirements"][i]

                with placeholder.container():
                    col_req, col_code = st.columns([1, 1])

                    with col_req:
                        form_key  = f"{selected_module}_form_{i}"
                        title_key = f"title_{selected_module}_{i}"
                        body_key  = f"body_{selected_module}_{i}"

                        with st.form(form_key, clear_on_submit=False):
                            st.text_input("Requirement Title", value=req["Function"],      key=title_key)
                            st.text_area("Requirement Text",   value=req["Requirements"],
                                         key=body_key, height=auto_height(req["Requirements"]))

                            btn_col, approve_col = st.columns([1, 1])
                            with btn_col:
                                submitted = st.form_submit_button("Apply changes")
                            with approve_col:
                                approved_now = st.checkbox(
                                    "User approved",
                                    value=req.get("Approved", False),
                                    key=f"approve_{selected_module}_{i}",
                                )

                            if submitted:
                                st.session_state["requirements"][i].update({
                                    "Function":     st.session_state[title_key],
                                    "Requirements": st.session_state[body_key],
                                    "Applied":      True,
                                    "Approved":     approved_now,
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

    # ── Persistent requirements rendering ─────────────────────────────────────
    if (st.session_state["generation_completed"]
            and st.session_state["requirements"]
            and not _did_generate_this_run):
        st.subheader(f"Requirements for **{selected_module}**")

        for i, fn in enumerate(st.session_state.get("functions", [])):
            req = st.session_state["requirements"].get(i)
            if req is None:
                continue

            col_req, col_code = st.columns([1, 1])

            with col_req:
                form_key  = f"{selected_module}_form_{i}"
                title_key = f"title_{selected_module}_{i}"
                body_key  = f"body_{selected_module}_{i}"

                with st.form(form_key, clear_on_submit=False):
                    st.text_input("Requirement Title", value=req["Function"],      key=title_key)
                    st.text_area("Requirement Text",   value=req["Requirements"],
                                 key=body_key, height=auto_height(req["Requirements"]))

                    btn_col, approve_col = st.columns([1, 1])
                    with btn_col:
                        submitted = st.form_submit_button("Apply changes")
                    with approve_col:
                        approved_now = st.checkbox(
                            "User approved",
                            value=req.get("Approved", False),
                            key=f"approve_{selected_module}_{i}",
                        )

                    if submitted:
                        st.session_state["requirements"][i].update({
                            "Function":     st.session_state[title_key],
                            "Requirements": st.session_state[body_key],
                            "Applied":      True,
                            "Approved":     approved_now,
                        })
                        st.success("Changes applied")

                st.markdown(format_globals(req["Globals"]))
                st.markdown(f"**Response time:** {round(req['ModelResponseTimeSec'], 2)} s")

            with col_code:
                with st.expander("Code Reference", expanded=True):
                    st.code(fn["content"], language="c")

            st.divider()

    # ── Export Section ────────────────────────────────────────────────────────

    if st.session_state["requirements"]:
        st.subheader("📤 Export Stage 1 Outputs")
        total_time = sum(st.session_state["inference_times"])
        st.markdown(f"**Total inference time:** {round(total_time / 60, 2)} min")

        _total     = len(st.session_state["requirements"])
        _approved  = sum(1 for r in st.session_state["requirements"].values() if r.get("Approved"))
        if _approved == _total:
            st.success(f"✅ All {_total} requirements approved — ready for Stage 2.")
        else:
            st.warning(f"⏳ {_approved} / {_total} requirements approved. "
                       "Check **User approved** and click **Apply changes** on every requirement "
                       "before proceeding to Stage 2.")

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

        st.info("➡ Use the **Stage 2** tab to convert these LLRs into Jama-compliant LLRs.")
    else:
        st.info("No requirements yet. Click **Generate LLRs** to start.")


# ═════════════════════════════════════════════════════════════════════════════
#  STAGE 2 — LLR → Jama LLR
# ═════════════════════════════════════════════════════════════════════════════

with tab2:
    st.header("Stage 2: LLRs → Jama-Compliant LLRs")
    st.markdown(
        "Upload a previously saved CSV file of LLRs "
        "or use the existing output from Stage 1. The LLM rewrites each draft into "
        "formal Jama-compliant **shall** statements."
    )

    st.caption(f"Model: `{STAGE2_MODEL}` — outputs saved to `artifacts/`")
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
            _total    = len(st.session_state["requirements"])
            _approved = sum(1 for r in st.session_state["requirements"].values() if r.get("Approved"))

            if _approved < _total:
                st.error(
                    f"🔒 **Approval required:** {_approved} / {_total} requirements approved in Stage 1. "
                    "Go back to Stage 1, check **User approved** on every requirement, and click "
                    "**Apply changes** before proceeding."
                )
            else:
                rows = [
                    {"Function Name": r["Function"], "Requirements": r["Requirements"]}
                    for r in st.session_state["requirements"].values()
                ]
                groups_df = pd.DataFrame(rows)
                st.success(f"✅ All {len(groups_df)} function(s) approved and loaded from Stage 1.")
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

        # Reset timer state for this batch.
        st.session_state["stage2_inference_times"] = []
        batch_start = time.time()

        for idx, group in enumerate(function_groups):
            status_box.info(f"Rewriting [{idx+1}/{len(function_groups)}]: **{group.function_name}**…")

            t0 = time.time()
            resp = requests.post(
                f"{API_URL}/rewrite",
                json={
                    "function_name": group.function_name,
                    "draft":         group.draft,
                },
                timeout=300,
                headers=_api_headers()
            )
            elapsed = time.time() - t0
            st.session_state["stage2_inference_times"].append(elapsed)

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

        batch_elapsed = time.time() - batch_start
        status_box.success(
            f"✅ {len(hlrs_all)} LLR(s) generated from {len(function_groups)} function(s) "
            f"in {round(batch_elapsed / 60, 2)} min."
        )

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

        _stage2_times = st.session_state.get("stage2_inference_times", [])
        if _stage2_times:
            _total_s2 = sum(_stage2_times)
            st.markdown(
                f"**Total inference time:** {round(_total_s2 / 60, 2)} min  "
                f"({len(_stage2_times)} call(s), avg {round(_total_s2 / len(_stage2_times), 2)} s)"
            )

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

        if st.button("📊 Save Jama Excel (.xlsx)", type="primary"):
            from src.pipelines.exporter import export_to_excel

            stem      = groups_df["Function Name"].iloc[0].split()[0] if groups_df is not None else "output"
            xlsx_path = _artifacts_path(f"{stem}_Jama_LLR.xlsx")
            export_to_excel(result, str(xlsx_path), filename=stem)
            st.success(f"Saved → `{xlsx_path}`")
    elif groups_df is not None:
        st.info("Click **Generate Jama LLRs** to start Stage 2.")
    else:
        st.info("Provide LLR input above to begin.")
