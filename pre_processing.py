import pycparser
from pycparser import c_parser, c_ast, parse_file
import subprocess
import regex as re
import os
from pathlib import Path

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