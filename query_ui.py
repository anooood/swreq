# ui_app.py
import streamlit as st
import requests
import glob, os
from pathlib import Path
import pycparser
import subprocess
from pycparser import c_parser, c_ast, parse_file
import regex as re

SERVER_URL = "http://<server-ip>:8000/generate"

# ───── Collect module names from filenames ─────
Git_folder = "P3_MCP_Application/cmake-src/src"
modules = [Path(f).name for f in glob.glob(Git_folder+"/*.c")]

st.title("MCP Application Requirements Generator")

selected_module = st.selectbox("Select a system/module:", modules)
cleaned_module = selected_module.replace('.c','')

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

# ───── Define Prompt for Generating Function Name (Used as Requirements Header) ─────
name_prompt_template = """
You are tasked with generating a **requirements heading** derived from a function name.

### Input
- Module comments (for reference only, do not copy directly):
{header_comments}
- Function name in code syntax:
{function_name}

### Task
Transform the function name into a **short, human-readable requirements heading**.  
The heading must be a **noun phrase**.

### Strict Output Rules
1. Output **only one phrase**.
2. The phrase must be in **title case** (capitalize significant words).
3. Do **not** output verbs or action-style phrasing (e.g., "Handles...", "Processes...").
4. Do **not** number, list, or bullet the output.
5. Do **not** add quotes, colons, punctuation, or extra text.
6. Do **not** include the original function name syntax.

### Output
"""

# ───── Define Prompt for Generating Software Requirements from Function ─────
reqs_prompt_template = """
You are an **expert system engineer** specialized in writing high-level software requirements from implementation code.
Treat the provided function implementation as the sole source of truth and produce only natural-language requirements.

### Inputs (informational)
- function_name: the implementation name (do not use this in outputs).
- function_code: the implementation to analyze.

### Task
Analyze the provided function code and generate a numbered list of high-level software requirements that are directly supported by the code.

### Output Format (strict — exact, no exceptions)
1. Output only a numbered list (1., 2., 3., ...), one requirement per line.
2. Each requirement must be exactly one sentence in plain natural language.
3. Each sentence MUST begin with one of these prefixes: "The system shall", "The module shall", or "The component shall".
4. Do not include any additional text, headings, examples, code, or commentary.

### Mandatory Rules (strict — no exceptions)
- Do NOT mention or copy any identifiers from the code (function names, variable names, parameter names, type names).
- Do NOT use code keywords, syntax, or code-like fragments (for example: if, while, ==, //, ->).
- Do NOT use phrases such as "the function does", "this function", "callers", or similar implementation-level wording.
- Do NOT describe algorithmic steps, internal loops, or implementation strategies; describe only observable behavior and obligations.
- Every requirement must be directly supported by behavior or constants present in the provided code; do not invent capabilities or assumptions not present in the code.
- Requirements must be unique and non-overlapping.

### What to describe (priority order)
- Observable functionality and outcomes.
- Inputs and outputs in conceptual terms (types, formats, ranges) — without variable names.
- Constraints, limits, thresholds, timeouts, and formats (express numeric values in natural language).
- Error handling and failure behavior (what observable error or response occurs).

### If nothing determinable
If no requirements can be reasonably inferred from the code, output exactly one item:
1. The system shall have no determinable requirements based on the provided code.

### Provided data
Function Name: {function_name}
Function Code:
{function_code}

### Requirements:
"""
    
if st.button("Generate Requirements"):

    ### Pre-process selected module and generate function chunks
    print(selected_module)
    output_file, input_file = pre_processing(selected_module)
    functions = generate_functions(output_file, input_file)
    header_comments = extract_header_comments(input_file)
    
    st.subheader(f"Requirements for **{selected_module}**")

    with st.spinner("Generating requirements..."):

        ### Loop through extracted function chunks
        for i in range(len(functions)):
            print(functions[i]['name'])

            ### Update prompts
            name_prompt = name_prompt_template.format(
                header_comments = header_comments,
                function_name = functions[i]['name']
            )
            reqs_prompt = reqs_prompt_template.format(
            function_name = functions[i]['name'],
            function_code = functions[i]['content']
            )

            try:

                ### Query LLM API
                response = requests.post(
                    "http://127.0.0.1:8000/generate",
                    json={"prompts": [name_prompt,reqs_prompt]},
                    timeout=300  # allow long responses
                )

                ### Printing response in streamlit UI
                if response.status_code == 200:
                    data = response.json()
                    st.markdown(data['requirements'][0])  # smaller than subheader
                    st.write(data['requirements'][1])
                    with st.expander("Code Reference"):
                        st.code(functions[i]['content'], language="c")
                    st.divider()
                else:
                    st.error(f"Error {response.status_code}: {response.text}")
            except Exception as e:
                st.error(f"Request failed: {e}")