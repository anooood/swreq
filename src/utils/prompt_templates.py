# ───── Define Prompt for Generating Function Name (Used as Requirements Header) ─────
name_prompt_template = """
You are tasked with generating a **requirements heading** derived from a function name.

### Input
- Module comments (for reference only, do not copy directly):
{header_comments}
- Function name in code syntax:
{function_name}

### Task
Transform the function name into a **short, human-readable requirements heading** that is a **pure noun phrase**.

### Linguistic Constraints (Mandatory)
- The output must contain **ONLY nouns, adjectives, or prepositional phrases**.
- **NO verbs of any kind** are allowed, including:
  - Base verbs (e.g., Handle, Process, Validate)
  - Gerunds ending in “-ing” (e.g., Handling, Processing)
  - Verb-derived phrases implying action
- If the function name implies an action, **abstract it into a conceptual noun** instead.

### Strict Output Rules
1. Output **exactly one phrase**.
2. Use **title case** (capitalize significant words).
3. The phrase must represent a **concept, entity, or state**, not an action.
4. Do **not** number, list, or bullet the output.
5. Do **not** add quotes, punctuation, or extra text.
6. Do **not** include the original function name syntax.

### Output (ONLY THE PHRASE):
"""

# ───── Define Prompt for Generating Software Requirements from Function ─────
reqs_prompt_template = """
ROLE
You are an expert systems engineer converting implementation code into **detailed** high-level software requirements.
Use the provided function code as the **only source of truth**. Do not infer, assume, or invent behavior.

INFORMATION PROVIDED
- A C function
- A dictionary mapping global variables to their numerical values
- Header comments describing the module and function purpose (when available)

TASK
Generate software requirements that describe the externally observable behavior of the function.
You MUST follow the steps below in order and complete each step before moving to the next.

────────────────────────────
STEP 0 — Context Assimilation
Read the HEADER COMMENTS section below (if non-empty) before processing any code line.
Extract the following and hold them in memory for STEP 4:
  - Module purpose: what the module is responsible for
  - Function purpose: what this specific function is supposed to achieve
  - Domain terminology: any named concepts, signals, states, or thresholds mentioned

Use this context in STEP 4 to:
  - Replace generic phrases with precise domain terms from the header
  - Clarify the intent of ambiguous lines
  - Preserve all numeric values — context only clarifies language, never replaces numbers

If HEADER COMMENTS is empty, skip this step and proceed with the function name as the only context.

────────────────────────────
STEP 1 — Line Iteration
Iterate through the C function **line by line from top to bottom**.
Do NOT skip or merge any line at this stage (STRICT).

────────────────────────────
STEP 2 — Trivial Line Filter  ← MANDATORY GATE
Before doing anything else with the current line, decide: is this line trivial?

A line is trivial if it ONLY does one of the following:
  - Calls sprintf, snprintf, or any string-formatting function
  - Reads or writes any variable named AuxStr (in any form: AuxStr, auxstr, AUXSTR, etc.)
  - Calls printf, fprintf, syslog, or any logging/debug macro
  - Sets or clears a flag to a fixed value (e.g. flag = 0, is_ready = TRUE)
  - Increments or decrements a counter (e.g. count++, retry -= 1)
  - Clears or zeroes a buffer or struct (e.g. memset, = {{0}})
  - Initialises a local scratch variable with no externally observable effect

If the line is trivial → **produce NOTHING. No requirement, no comment, no placeholder. Absolute silence.**
If the line is NOT trivial → proceed to STEP 3.

Examples of trivial lines that MUST produce no output:
  sprintf(AuxStr, "Temp: %d", val);       → nothing
  memset(AuxStr, 0, sizeof(AuxStr));      → nothing
  error_flag = 0;                         → nothing
  retry_count++;                          → nothing

────────────────────────────
STEP 3 — Global Value Resolution
For the current line:
- Identify all global variables referenced on that line.
- Replace each global variable with its numerical value using the provided GLOBAL VARIABLES (value mapping) dictionary.
- Global values may be integers, floating-point numbers, decimals, or hexadecimal values.
- Hexadecimal values MUST be preserved exactly and SHALL NOT be converted.

────────────────────────────
STEP 4 — Numeric Extraction (STRICT)
For the current line:
- Identify ALL numerical values:
  - Inline numeric literals
  - Resolved global variable values
- Every identified numerical value **MUST appear explicitly in the requirement**.

────────────────────────────
STEP 5 — Requirement Writing
For the current line, write ONE software requirement that:
- Uses a **shall** statement
- Contains numeric values that were extracted in STEP 4 (STRICT)
- Does **not** mention any code identifiers or fragments/substrings of them (case-insensitive).
- Avoids all code patterns: `.`, `->`, `::`, `[]`, `()`, snake_case, camelCase, PascalCase, or UPPER_SNAKE.
- Replaces identifiers with conceptual phrases informed by the header context
  (e.g., "an internal counter" → "the fault counter", "a delay threshold" → "the arming delay threshold")

Example:
If the C function line is:

`IF_SendMsg(FUZE_SETTINGS_ID, (uint8_t *) &SS_FUZE.FuzeSettings, sizeof(SS_FUZE.FuzeSettings));`
And `FUZE_SETTINGS_ID = 0x20` in GLOBAL VARIABLES, then write:

"The Fuze module shall send a message to the IF with the updated fuze settings using the ID 0x20"

────────────────────────────
OUTPUT FORMAT
- Numbered list (1., 2., 3., …)
- Separate distinct requirement branches into newlines.
- One sentence per requirement
- One requirement per non-trivial source line
- No explanations, comments, or extra text

────────────────────────────
FINAL VERIFICATION (MANDATORY)
Before producing the final output, verify that:
- Trivial lines (per STEP 2) produced NO output whatsoever — not even a placeholder.
- Each non-trivial line produced exactly one requirement.
- All global variables were replaced with their **numerical values** and are mentioned in the requirements (STRICT).
- All numerical values extracted in STEP 4 appear in the requirements along with their units, if applicable (e.g. hexadecimal, milliseconds..) (STRICT)
- All requirements are written as **shall** statements
- No code syntax, variable names, or identifiers appear anywhere in the output. REWRITE CONCEPTUALLY.
- Domain terms from the header comments are used where they sharpen meaning.

────────────────────────────
HEADER COMMENTS:
{header_comments}

────────────────────────────
C FUNCTION:
{C_FUNCTION}

GLOBAL VARIABLES (value mapping):
{GLOBAL_VARIABLES}

────────────────────────────
FINAL OUTPUT:
"""

identifier_rewrite_prompt = """
ROLE
You are an expert systems engineer.

TASK
Using the INITIAL REQUIREMENTS as context, rewrite each identifier in the LEAKED IDENTIFIERS list conceptually in clear, natural English suitable for a formal requirements document.
Ensure the replacements make sense in the sentence and it is cohesive.

OUTPUT
Return ONLY a JSON object mapping each original identifier to its rewritten version.
Do not include the original identifiers as values, explanations, or the full requirements.
Do not output anything outside the JSON object.

Example:
Input:  ["Weapon.Wind", "TelemetryBuild2HzBlock2"]
Output: {{"Weapon.Wind": "weapon wind", "TelemetryBuild2HzBlock2": "telemetry build 2Hz block"}}

INITIAL REQUIREMENTS:
{REQUIREMENTS}

LEAKED IDENTIFIERS:
{VARIABLES}

CONCEPTUAL IDENTIFIERS:
"""

trivial_filter_prompt = """"""

_REWRITE_PROMPT = """
You are a systems-engineering expert rewriting requirements for a Jama requirements database.

You are given a SOFTWARE FUNCTION NAME and a DRAFT REQUIREMENTS STRING for that function.
The draft may contain one or several obligations written informally or non-compliantly.

YOUR TASKS:
1. Read the draft carefully and identify every distinct obligation or requirement within it.
2. Decide how many separate Jama requirements are needed (one per distinct obligation).
   - Split if two or more clearly different obligations are bundled together.
   - Keep as one if the draft expresses a single coherent requirement.
3. For each requirement, rewrite the Description as a Jama-compliant "shall" statement.
4. PRESERVE ALL DETAILS from the original draft — do not drop any information,
   numeric values, named interfaces, timing constraints, or conditions (IMPORTANT)

REWRITING RULES for every Description:
  R1. Must start with "The system shall" or "The <subsystem> shall"
  R2. Must be a SINGLE sentence — one obligation per requirement
      (never use "and shall", "and must", "as well as" to chain two obligations)
  R3. Must be verifiable — a tester can unambiguously pass or fail it
  R4. Must be unambiguous — replace vague words:
        "appropriate" → specific criterion
        "adequate"    → measurable threshold
        "support"     → specific capability
        "as needed"   → defined condition
        "properly"    → explicit acceptance criterion
        "etc."        → enumerate all items or use a defined reference
  R5. Must describe WHAT the system shall do, not HOW it shall do it
  R6. Use active voice ("the system shall X", not "X shall be done")
  R7. Mention all numerical values (e.g. conversions, timing thresholds..) from the original draft (IMPORTANT)

For EACH rewritten requirement output a JSON object with these keys:
  "name"                    : short noun-phrase title (max 5 words, no verbs)
  "description"             : the rewritten "shall" statement (apply rules R1-R7)
  "verification_method"     : one of [Analysis, Inspection, Test, Demonstration]
                            Use the VERIFICATION METHODS REFERENCE below to select the correct method.
                                Test          → exercised and measured at runtime
                                Inspection    → verified by reviewing design or code
                                Analysis      → verified by calculation or modelling
                                Demonstration → verified by operating the system
  "requirement_type"        : one of [Functional Req., Performance Req., Interface Req., Constraint Req.]
                            Use the REQUIREMENT TYPE REFERENCE below to select the correct type.
  "engineering_discpline"   : "Software"

Respond ONLY with a JSON array of such objects.
Do NOT include any text outside the JSON array.

{verification_methods_context}

{requirement_type_context}

Function name        : {function_name}
Draft requirements   : {draft}
"""