import pycparser
from pycparser import c_parser, c_ast, c_generator, plyparser, parse_file
import subprocess
import regex as re
import os
from pathlib import Path
from typing import List, Union
import difflib

Git_folder = "P3_MCP_Application/cmake-src/src"

# ───── Class to Extract Functions from C Module ─────
class CodeElement:
    def __init__(self, name, type, start_line, end_line, code_snippet):
        self.name = name
        self.type = type        # 'function' or 'global'
        self.start_line = start_line
        self.end_line = end_line        
        self.code_snippet = code_snippet

def extract_code_elements(filename):
    ast = parse_file(filename, use_cpp=False)
    elements = []
    
    class Visitor(c_ast.NodeVisitor):
        def visit_FuncDef(self, node):
            start_line = node.decl.coord.line
            end_line = self._get_end_line(node.body)

            snippet = str(node.body)

            elements.append(CodeElement(
                name=node.decl.name,
                type="function",
                start_line=start_line,
                end_line=end_line,
                code_snippet=snippet
            ))
            self.generic_visit(node)

        def visit_Decl(self, node):
            if isinstance(node.type, c_ast.TypeDecl) and node.coord:
                elements.append(CodeElement(
                    name=node.name,
                    type="global",
                    start_line=node.coord.line,
                    end_line=node.coord.line,
                    code_snippet=str(node)
                ))

        def _get_end_line(self, node):
            """Recursively find the maximum line number inside a function body."""
            max_line = node.coord.line if node.coord else -1
            for _, child in node.children():
                child_line = self._get_end_line(child)
                max_line = max(max_line, child_line)
            return max_line
        
    Visitor().visit(ast)
    return elements

class ContextExtractor(c_ast.NodeVisitor):
    def __init__(self):
        self.typedefs = {}
        self.enums = {}
        self.structs = {}
        self.globals = {}
        self.function_decls = {}   # prototypes
        self.function_defs = {}    # definitions
        self.generator = c_generator.CGenerator()

    def visit_Typedef(self, node):
        self.typedefs[node.name] = self.generator.visit(node)

    def visit_Enum(self, node):
        name = node.name or "<anonymous_enum>"
        self.enums[name] = self.generator.visit(node)

    def visit_Struct(self, node):
        name = node.name or "<anonymous_struct>"
        self.structs[name] = self.generator.visit(node)

    def visit_Decl(self, node):
        # Function prototype
        if isinstance(node.type, c_ast.FuncDecl):
            func_name = node.name
            if func_name:
                self.function_decls[func_name] = node
            return

        # Global variable
        if isinstance(node.type, (c_ast.TypeDecl, c_ast.PtrDecl)):
            if not isinstance(node.type.type, c_ast.FuncDecl):
                self.globals[node.name] = self.generator.visit(node)

    def visit_FuncDef(self, node):
        func_name = node.decl.name
        self.function_defs[func_name] = node

def get_comment_positions(source_code):
    code_lines = source_code.splitlines()
    comments = re.findall(r'//.*|/\*[\s\S]*?\*/', source_code, flags=re.MULTILINE)

    # Extract line numbers (optional) to map to AST nodes
    comment_positions = []
    for comment in comments:
        # Find line numbers where this comment appears
        for i, line in enumerate(code_lines):
            if comment in line:
                comment_positions.append((i+1, comment))  # 1-based line number
                break
    return comment_positions
  
def add_comments(func_code, functions, comment_positions):
    func_code_comments = []
    func_code_startline = functions['start line']

    code_line = func_code_startline
    for line in func_code.split('\n'):
        func_code_comments.append(line)
        code_line+=1
        if code_line <= functions['end line']:
            for pos in comment_positions:
                if pos[0] == code_line:
                    func_code_comments.append(pos[1])
                    code_line+=1

    return '\n'.join(func_code_comments)

def tokenize_c(src: str):
    """
    Very simple C-like tokenizer:
    - multi-char operators: ==, !=, >=, <=, ->, ++, --, &&, ||
    - hex numbers: 0xFF
    - decimals/ints: 3.14, 42
    - identifiers: IMU_ADDRESS, msg1, etc.
    - everything else: single non-space char
    """
    token_re = re.compile(
        r"""
        (==|!=|>=|<=|->|\+\+|--|&&|\|\|)   # multi-char ops
        |(0x[0-9A-Fa-f]+)                 # hex
        |(\d+\.\d+|\d+)                   # numbers
        |([A-Za-z_]\w*)                   # identifiers
        |([^\s])                          # single non-space char
        """,
        re.VERBOSE,
    )

    tokens = []
    for match in token_re.findall(src):
        # each match is a tuple, only one group is non-empty
        for g in match:
            if g:
                tokens.append(g)
    return tokens

def get_identifier_mapping(orig_src: str, emb_src: str):
    """
    orig_src : original C source (with macros / symbolic constants)
    emb_src  : embedded/expanded C source (with literal values)

    Returns dict like:
        {"IMU_ADDRESS": "0x0A", "FALSE": "0", "IMU_SF_ANGLE_X": "(1.0 / 1073741824)", ...}
    """
    t1 = tokenize_c(orig_src)
    t2 = tokenize_c(emb_src)

    sm = difflib.SequenceMatcher(a=t1, b=t2, autojunk=False)
    ident_re = re.compile(r'^[A-Za-z_]\w*$')

    mapping = {}

    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        # we care about places where something was *replaced*
        if tag == "replace":
            left = t1[i1:i2]
            right = t2[j1:j2]

            # simple & common case:
            #   one identifier on the left -> some expression on the right
            if len(left) == 1 and ident_re.match(left[0]):
                name = left[0]
                value_expr = " ".join(right)
                mapping[name] = value_expr

    return mapping

# ───── Function to Pre-process C File ─────
def pre_processing(selected_module):
    pkg_path = os.path.dirname(pycparser.__file__)
    fake_include_path = os.path.join(pkg_path, 'utils', 'fake_libc_include')

    input_file = Git_folder+'/'+selected_module
    output_file = 'C_Parsed/'+selected_module.replace('.c','.i')

    cmd = [
        "gcc",
        "-E",
        '-nostdinc',
        f'{input_file}',
        '-I./include',
        '-I./P3_MCP_Application/cmake-src/src/AutoPilot',
        '-I./P3_MCP_Application/cmake-src/src/Common',
        '-I./P3_MCP_Application/cmake-src/src/DataControl',
        '-I./P3_MCP_Application/cmake-src/src/GNSS',
        '-I./P3_MCP_Application/cmake-src/src/Guidance',
        '-I./P3_MCP_Application/cmake-src/src/Nav',
        f'-I{fake_include_path}',
    ]

    with open(output_file, "w") as f:
        subprocess.run(cmd, stdout=f, check=True)

    return output_file, input_file

# ───── Functions to Extract C Function Chunks ─────
def extract_function_snippet(c_file, start_line, end_line):
    with open(c_file, "r", errors="ignore") as f:
        lines = f.readlines()
    
    # slice lines (list is 0-based, start_line is 1-based)
    snippet_lines = lines[start_line-1:end_line+1]
    
    return "".join(snippet_lines)

def generate_functions(output_file, input_file):
    elements = extract_code_elements(output_file)
    functions = []
    for e in elements:
        if e.type == 'function':
            print(f"{e.type} {e.name} at line {e.start_line}:{e.end_line}")
            content = extract_function_snippet(input_file, e.start_line, e.end_line)
            function_dict = {
                'name': e.name,
                'content': content,
                'start line': e.start_line,
                'end line': e.end_line,
                'file': Path(input_file).name
            }
            functions.append(function_dict)
    return functions

# ───── Function to Extract Header Comments Section ─────
def extract_header_comments(input_file):
    with open(input_file, "r", errors="ignore") as f:
        code = f.read()
    
    # Look for /**/ style at the very top
    match = re.match(r"(/\*.*?\*/)", code, re.DOTALL)
    if match:
        return match.group(1).strip()
    
    # Fallback: consecutive // comments at the top
    comments = []
    for line in code.splitlines():
        if line.strip().startswith("//"):
            comments.append(line.strip())
        elif comments:
            break
    return "\n".join(comments)

# ───── Function to Clean Header Comments Section ─────

def clean_header_comment_text_only(header: str) -> str:
    header = header.replace('\r\n', '\n').replace('\r', '\n')
    lines = header.split('\n')
    
    cleaned = []
    for raw in lines:
        # Drop full-line banner delimiters and star bars
        if re.match(r'^\s*/\*+[*\s/]*\s*$', raw):
            continue
        if re.match(r'^\s*\*+\s*/\s*$', raw):
            continue
        if re.match(r'^\s*\*{10,}\s*$', raw):  # long star-only lines
            continue
        
        # Strip leading '*' and whitespace
        line = re.sub(r'^\s*\*\s?', '', raw).rstrip()
        
        # Remove Company lines
        if re.search(r'\bCompany\b', line, re.IGNORECASE):
            continue
        if re.match(r'^\s*Company\s*:?', line, re.IGNORECASE):
            continue
        
        # Remove dashed separator lines, only '-' chars possibly with spaces
        if re.match(r'^\s*-{2,}\s*$', line):
            continue
        
        # Remove pure ellipsis lines ("...") possibly with spaces
        if re.match(r'^\s*\.{3}\s*$', line):
            continue
        
        cleaned.append(line)
    
    # Collapse blanks
    out_lines = []
    prev_blank = False
    for l in cleaned:
        if not l.strip():
            if not prev_blank:
                out_lines.append('')
            prev_blank = True
        else:
            out_lines.append(l.strip())
            prev_blank = False
    
    # Trim leading/trailing blanks
    while out_lines and not out_lines[0].strip():
        out_lines.pop(0)
    while out_lines and not out_lines[-1].strip():
        out_lines.pop()
    
    return '\n'.join(out_lines)

def clean_header_comments(headers: Union[str, List[str]]) -> Union[str, List[str]]:
    if isinstance(headers, str):
        return clean_header_comment_text_only(headers)
    return [clean_header_comment_text_only(h) for h in headers]

# ───── Function to Extract Function Description from Header Comments ─────

def get_header_function_descriptions(header_text: str):
    """
    Returns a list of descriptions in the same order as they appear under Function(s):
    index 0 -> description of first function
    index 1 -> description of second function
    ...
    """
    pattern = r"^\s*-\s*[A-Za-z_]\w*\s*:\s*(.+)$"
    return [m.group(1).strip() for m in re.finditer(pattern, header_text, flags=re.MULTILINE)]

def resolve_struct_ref(node):
    """
    Recursively resolve a StructRef into a dotted string:
    item.flags.bits.windok
    """
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
        self.ids = set()
        self.func_calls = set()
        self.struct_accesses = set()
        self.local_vars = set()
        self.global_names = global_names

    def visit_ID(self, node):
        self.ids.add(node.name)

    def visit_FuncCall(self, node):
        if isinstance(node.name, c_ast.ID):
            self.func_calls.add(node.name.name)
        if node.args:
            self.visit(node.args)

    def visit_Decl(self, node):
        # Skip function declarations
        if isinstance(node.type, c_ast.FuncDecl):
            return

        # Local variable declaration
        if node.name and node.name not in self.global_names:
            self.local_vars.add(node.name)

        self.generic_visit(node)

    def visit_StructRef(self, node):
        full_path = resolve_struct_ref(node)
        self.struct_accesses.add(full_path)
        self.generic_visit(node)

    def generic_visit(self, node):
        for _, child in node.children():
            self.visit(child)

def extract_function_parameters(func_def):
    params = set()
    func_decl = func_def.decl.type
    if func_decl.args:
        for param in func_decl.args.params:
            if isinstance(param, c_ast.Decl):
                params.add(param.name)
    return params

def extract_function_context(extractor):
    context = {}

    global_names = set(extractor.globals.keys())
    function_names = set(extractor.function_decls.keys()) | set(extractor.function_defs.keys())

    for func_name, func_node in extractor.function_defs.items():
        visitor = FunctionUsageVisitor(global_names)

        # Visit function body
        visitor.visit(func_node.body)

        # Extract parameters
        params = extract_function_parameters(func_node)

        used_structs = {
            s for s in visitor.struct_accesses
            if s.split(".")[0] in extractor.globals
        }

        context[func_name] = {
            "globals_used": sorted(visitor.ids & global_names),
            "locals_declared": sorted(visitor.local_vars | params),
            "functions_called": sorted(visitor.func_calls & function_names),
            "used_structures": used_structs
        }

    return context