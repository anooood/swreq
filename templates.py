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