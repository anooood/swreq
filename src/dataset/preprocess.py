"""
src/dataset/preprocess.py
--------------------------
C source code parsing and preprocessing pipeline.
Uses pycparser + GCC preprocessor to extract functions, globals, comments,
and identifier mappings from C modules.
"""

import os
import subprocess
import difflib
from pathlib import Path
from typing import List, Union

import pycparser
import regex as re
from pycparser import c_ast, c_generator, parse_file

# ---------------------------------------------------------------------------
# Config — can be overridden by caller
# ---------------------------------------------------------------------------

# Default source folder. Empty by default — callers (e.g. the Streamlit
# UI after a codebase upload) pass a `git_folder` argument to pre_processing()
# at runtime. Override globally with the SWREQ_GIT_FOLDER env var if needed.
GIT_FOLDER   = os.getenv("SWREQ_GIT_FOLDER", "")
PARSED_DIR   = "data/C_Parsed"


# ---------------------------------------------------------------------------
# Code element containers
# ---------------------------------------------------------------------------

class CodeElement:
    def __init__(self, name, type, start_line, end_line, code_snippet):
        self.name         = name
        self.type         = type          # 'function' or 'global'
        self.start_line   = start_line
        self.end_line     = end_line
        self.code_snippet = code_snippet


def extract_code_elements(filename):
    ast      = parse_file(filename, use_cpp=False)
    elements = []

    class Visitor(c_ast.NodeVisitor):
        def visit_FuncDef(self, node):
            start_line = node.decl.coord.line
            end_line   = self._get_end_line(node.body)
            elements.append(CodeElement(
                name=node.decl.name, type="function",
                start_line=start_line, end_line=end_line,
                code_snippet=str(node.body),
            ))
            self.generic_visit(node)

        def visit_Decl(self, node):
            if isinstance(node.type, c_ast.TypeDecl) and node.coord:
                elements.append(CodeElement(
                    name=node.name, type="global",
                    start_line=node.coord.line, end_line=node.coord.line,
                    code_snippet=str(node),
                ))

        def _get_end_line(self, node):
            max_line = node.coord.line if node.coord else -1
            for _, child in node.children():
                max_line = max(max_line, self._get_end_line(child))
            return max_line

    Visitor().visit(ast)
    return elements


# ---------------------------------------------------------------------------
# AST context extractor
# ---------------------------------------------------------------------------

class ContextExtractor(c_ast.NodeVisitor):
    def __init__(self):
        self.typedefs       = {}
        self.enums          = {}
        self.structs        = {}
        self.globals        = {}
        self.function_decls = {}
        self.function_defs  = {}
        self.generator      = c_generator.CGenerator()

    def visit_Typedef(self, node):
        self.typedefs[node.name] = self.generator.visit(node)

    def visit_Enum(self, node):
        self.enums[node.name or "<anonymous_enum>"] = self.generator.visit(node)

    def visit_Struct(self, node):
        self.structs[node.name or "<anonymous_struct>"] = self.generator.visit(node)

    def visit_Decl(self, node):
        if isinstance(node.type, c_ast.FuncDecl):
            if node.name:
                self.function_decls[node.name] = node
            return
        if isinstance(node.type, (c_ast.TypeDecl, c_ast.PtrDecl)):
            if not isinstance(node.type.type, c_ast.FuncDecl):
                self.globals[node.name] = self.generator.visit(node)

    def visit_FuncDef(self, node):
        self.function_defs[node.decl.name] = node


# ---------------------------------------------------------------------------
# Comment utilities
# ---------------------------------------------------------------------------

def get_comment_positions(source_code):
    code_lines = source_code.splitlines()
    comments   = re.findall(r"//.*|/\*[\s\S]*?\*/", source_code, flags=re.MULTILINE)
    positions  = []
    for comment in comments:
        for i, line in enumerate(code_lines):
            if comment in line:
                positions.append((i + 1, comment))
                break
    return positions


def add_comments(func_code, functions, comment_positions):
    result   = []
    code_line = functions["start line"]
    for line in func_code.split("\n"):
        result.append(line)
        code_line += 1
        if code_line <= functions["end line"]:
            for pos in comment_positions:
                if pos[0] == code_line:
                    result.append(pos[1])
                    code_line += 1
    return "\n".join(result)


# ---------------------------------------------------------------------------
# Tokenizer & identifier mapping
# ---------------------------------------------------------------------------

def tokenize_c(src: str):
    token_re = re.compile(
        r"""
        (==|!=|>=|<=|->|\+\+|--|&&|\|\|)
        |(0x[0-9A-Fa-f]+)
        |(\d+\.\d+|\d+)
        |([A-Za-z_]\w*)
        |([^\s])
        """,
        re.VERBOSE,
    )
    tokens = []
    for match in token_re.findall(src):
        for g in match:
            if g:
                tokens.append(g)
    return tokens


def get_identifier_mapping(orig_src: str, emb_src: str) -> dict:
    t1       = tokenize_c(orig_src)
    t2       = tokenize_c(emb_src)
    sm       = difflib.SequenceMatcher(a=t1, b=t2, autojunk=False)
    ident_re = re.compile(r"^[A-Za-z_]\w*$")
    mapping  = {}
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == "replace":
            left  = t1[i1:i2]
            right = t2[j1:j2]
            if len(left) == 1 and ident_re.match(left[0]):
                mapping[left[0]] = " ".join(right)
    return mapping


# ---------------------------------------------------------------------------
# Preprocessing — GCC expand + pycparser parse
# ---------------------------------------------------------------------------

def pre_processing(selected_module: str, git_folder: str | None = None):
    """
    GCC-preprocess a C source file and write the result to data/C_Parsed/.
    Returns (output_file_path, input_file_path).

    Parameters
    ----------
    selected_module : filename of the .c file (e.g. "Wind.c")
    git_folder      : directory containing the .c file. Required — either
                      passed by the caller (Streamlit UI after codebase
                      upload) or set via the SWREQ_GIT_FOLDER env var.
    """
    src_folder = git_folder or GIT_FOLDER
    if not src_folder:
        raise ValueError(
            "No source folder provided. Pass `git_folder=...` or set the "
            "SWREQ_GIT_FOLDER environment variable."
        )

    # Locate fake_libc_include — required by GCC -nostdinc to find stub
    # versions of <math.h>, <stdio.h>, etc., that pycparser can swallow.
    #
    # Search order:
    #   1. FAKE_LIBC_INCLUDE env var            (explicit override)
    #   2. <repo_root>/data/fake_libc_include   (bundled copy, used in Docker)
    #   3. pycparser source distribution        (local installs from GitHub)
    repo_root = Path(__file__).resolve().parents[2]
    candidates = [
        os.environ.get("FAKE_LIBC_INCLUDE"),
        str(repo_root / "data" / "fake_libc_include"),
        os.path.join(os.path.dirname(pycparser.__file__), "utils", "fake_libc_include"),
    ]
    fake_include = next((c for c in candidates if c and Path(c).exists()), None)
    if fake_include is None:
        raise RuntimeError(
            "fake_libc_include not found. Either:\n"
            "  • set FAKE_LIBC_INCLUDE to its absolute path,\n"
            "  • copy it to <repo_root>/data/fake_libc_include/, or\n"
            "  • install pycparser from source (pip install git+https://github.com/eliben/pycparser)."
        )

    input_file  = f"{src_folder}/{selected_module}"
    output_file = f"{PARSED_DIR}/{selected_module.replace('.c', '.i')}"
    os.makedirs(PARSED_DIR, exist_ok=True)

    # Auto-discover include directories: any subfolder of src_folder that
    # contains at least one .h file becomes a -I path. This replaces the
    # old hardcoded list (AutoPilot/Common/DataControl/...) so any codebase
    # layout works.
    include_dirs = [src_folder]
    src_path = Path(src_folder)
    if src_path.exists():
        for sub in src_path.rglob("*"):
            if sub.is_dir() and any(sub.glob("*.h")):
                include_dirs.append(str(sub))

    cmd = ["gcc", "-E", "-nostdinc", input_file]
    for inc in include_dirs:
        cmd.extend(["-I", inc])
    cmd.extend(["-I", fake_include])

    # Capture both streams so we can transcode stdout to clean UTF-8.
    # Using text=False keeps stdout as raw bytes — we don't trust GCC's
    # encoding choice when the input source might be Windows-1252.
    result = subprocess.run(cmd, capture_output=True, text=False)

    if result.returncode != 0:
        stderr_text = result.stderr.decode("utf-8", errors="replace")
        raise RuntimeError(
            f"GCC preprocessing failed for {input_file} "
            f"(exit {result.returncode}).\n"
            f"Command: {' '.join(cmd)}\n\n"
            f"GCC stderr:\n{stderr_text}"
        )

    # Decode GCC output as Latin-1 first (which always succeeds — every byte
    # 0x00–0xFF maps to a valid Latin-1 codepoint), then re-encode as UTF-8.
    # This converts characters like '°' (0xB0) into their proper UTF-8 form
    # instead of leaving them as raw Latin-1 bytes that confuse pycparser.
    preprocessed_text = result.stdout.decode("latin-1")
    with open(output_file, "w", encoding="utf-8") as f:
        f.write(preprocessed_text)

    return output_file, input_file


# ---------------------------------------------------------------------------
# Function extraction
# ---------------------------------------------------------------------------

def extract_function_snippet(c_file, start_line, end_line):
    with open(c_file, "r", errors="ignore") as f:
        lines = f.readlines()
    return "".join(lines[start_line - 1 : end_line + 1])


def generate_functions(output_file, input_file):
    elements  = extract_code_elements(output_file)
    functions = []
    for e in elements:
        if e.type == "function":
            content = extract_function_snippet(input_file, e.start_line, e.end_line)
            functions.append({
                "name":       e.name,
                "content":    content,
                "start line": e.start_line,
                "end line":   e.end_line,
                "file":       Path(input_file).name,
            })
    return functions


# ---------------------------------------------------------------------------
# Header comment helpers
# ---------------------------------------------------------------------------

def extract_header_comments(input_file):
    with open(input_file, "r", errors="ignore") as f:
        code = f.read()
    match = re.match(r"(/\*.*?\*/)", code, re.DOTALL)
    if match:
        return match.group(1).strip()
    comments = []
    for line in code.splitlines():
        if line.strip().startswith("//"):
            comments.append(line.strip())
        elif comments:
            break
    return "\n".join(comments)


def clean_header_comment_text_only(header: str) -> str:
    header = header.replace("\r\n", "\n").replace("\r", "\n")
    cleaned = []
    for raw in header.split("\n"):
        if re.match(r"^\s*/\*+[*\s/]*\s*$", raw):            continue
        if re.match(r"^\s*\*+\s*/\s*$", raw):                continue
        if re.match(r"^\s*\*{10,}\s*$", raw):                continue
        line = re.sub(r"^\s*\*\s?", "", raw).rstrip()
        if re.search(r"\bCompany\b", line, re.IGNORECASE):    continue
        if re.match(r"^\s*Company\s*:?", line, re.IGNORECASE): continue
        if re.match(r"^\s*-{2,}\s*$", line):                  continue
        if re.match(r"^\s*\.{3}\s*$", line):                  continue
        cleaned.append(line)

    out, prev_blank = [], False
    for l in cleaned:
        if not l.strip():
            if not prev_blank:
                out.append("")
            prev_blank = True
        else:
            out.append(l.strip())
            prev_blank = False

    while out and not out[0].strip():  out.pop(0)
    while out and not out[-1].strip(): out.pop()
    return "\n".join(out)


def clean_header_comments(headers: Union[str, List[str]]) -> Union[str, List[str]]:
    if isinstance(headers, str):
        return clean_header_comment_text_only(headers)
    return [clean_header_comment_text_only(h) for h in headers]


def get_header_function_descriptions(header_text: str):
    pattern = r"^\s*-\s*[A-Za-z_]\w*\s*:\s*(.+)$"
    return [m.group(1).strip() for m in re.finditer(pattern, header_text, flags=re.MULTILINE)]


# ---------------------------------------------------------------------------
# Function context extraction
# ---------------------------------------------------------------------------

def resolve_struct_ref(node):
    parts = []
    while isinstance(node, c_ast.StructRef):
        if isinstance(node.field, c_ast.ID):
            parts.append(node.field.name)
        node = node.name
    if isinstance(node, c_ast.ID):
        parts.append(node.name)
    return ".".join(reversed(parts))


class FunctionUsageVisitor(c_ast.NodeVisitor):
    def __init__(self, global_names):
        self.ids            = set()
        self.func_calls     = set()
        self.struct_accesses = set()
        self.local_vars     = set()
        self.global_names   = global_names

    def visit_ID(self, node):           self.ids.add(node.name)
    def visit_FuncCall(self, node):
        if isinstance(node.name, c_ast.ID):
            self.func_calls.add(node.name.name)
        if node.args:
            self.visit(node.args)

    def visit_Decl(self, node):
        if isinstance(node.type, c_ast.FuncDecl): return
        if node.name and node.name not in self.global_names:
            self.local_vars.add(node.name)
        self.generic_visit(node)

    def visit_StructRef(self, node):
        self.struct_accesses.add(resolve_struct_ref(node))
        self.generic_visit(node)

    def generic_visit(self, node):
        for _, child in node.children():
            self.visit(child)


def extract_function_parameters(func_def):
    params    = set()
    func_decl = func_def.decl.type
    if func_decl.args:
        for param in func_decl.args.params:
            if isinstance(param, c_ast.Decl):
                params.add(param.name)
    return params


def extract_function_context(extractor: ContextExtractor) -> dict:
    context        = {}
    global_names   = set(extractor.globals.keys())
    function_names = set(extractor.function_decls.keys()) | set(extractor.function_defs.keys())

    for func_name, func_node in extractor.function_defs.items():
        visitor = FunctionUsageVisitor(global_names)
        visitor.visit(func_node.body)
        params       = extract_function_parameters(func_node)
        used_structs = {s for s in visitor.struct_accesses if s.split(".")[0] in extractor.globals}

        # Keep the original dotted paths (e.g. "SS_FUZE.FuzeSettings.Delay")
        # so leak detection catches them if they appear verbatim, AND add
        # the individual components so partial leaks like just "FuzeSettings"
        # are also caught by Pass 2's identifier rewrite.
        used_struct_parts = used_structs | {part for s in used_structs for part in s.split(".")}

        context[func_name] = {
            "globals_used":       sorted(visitor.ids & global_names),
            "locals_declared":    sorted(visitor.local_vars | params),
            "functions_called":   sorted(visitor.func_calls & function_names),
            "used_structures":    used_struct_parts,
        }
    return context
