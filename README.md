# swreq — Software Requirements Generator

Parses C source code and produces formal, **Jama-compliant High-Level Requirements (HLR)** through a fully automated two-stage LLM pipeline.

All LLM inference is handled by a **local Ollama instance** — no cloud API keys, no model weights in the repo.

---

## How It Works

```
Stage 1 — Code Analysis
  C source file
      │  GCC preprocessor + pycparser
      ▼
  Per-function AST context
      │  Ollama LLM (name prompt + requirements prompt)
      ▼
  Low-Level Requirements (LLR)  →  saved as .docx + .csv

Stage 2 — Jama Compliance
  LLR CSV (from Stage 1 or uploaded)
      │  Ollama LLM (Jama rewrite prompt)
      ▼
  Jama-compliant HLRs (ID, shall statement, verification, type)
      │
      ▼
  Jama-importable Excel .xlsx + HLR .csv
```

Both stages run inside a single **Streamlit UI**. LLM calls are routed through a **FastAPI server** that acts as a thin proxy to Ollama.

---

## Project Structure

```
swreq/
├── run.sh                              ← Start/stop the entire application
│
├── api/
│   └── app.py                          ← FastAPI server (Ollama proxy)
│
├── notebooks/
│   └── llm_streamlit_app.py            ← Unified Streamlit UI (Stage 1 + 2)
│
├── src/
│   ├── dataset/
│   │   ├── preprocess.py               ← C parser, function extractor (pycparser)
│   │   └── dataset_loader.py
│   ├── pipelines/
│   │   ├── inference_pipeline.py       ← LLR → Jama HLR rewrite pipeline
│   │   ├── exporter.py                 ← MergeResult → Jama Excel workbook
│   │   └── hierarchy_sheet.py          ← Jama hierarchy sheet builder
│   ├── utils/
│   │   ├── prompt_templates.py         ← All LLM prompt templates
│   │   ├── word_output.py              ← Word document generator
│   │   ├── logger.py
│   │   └── config_utils.py
│   ├── models/
│   │   ├── base_model.py               ← OllamaLLM client class
│   │   └── finetune.py
│   └── evaluation/
│       └── eval_metrics.py
│
├── configs/
│   ├── model.yaml                      ← Ollama model + URL config
│   ├── data.yaml                       ← Input/output paths
│   └── train.yaml                      ← Fine-tuning config (placeholder)
│
├── data/
│   ├── P3_MCP_Application/cmake-src/src/   ← PUT YOUR C SOURCE FILES HERE
│   └── C_Parsed/                           ← Auto-generated preprocessed files
│
├── artifacts/                          ← All outputs saved here
│   ├── logs/                           ← api.log, ui.log
│   └── Experiments/
│
├── tests/
│   ├── development/
│   ├── sit/
│   ├── uat/
│   └── production/
│
├── scripts/
│   ├── main.py                         ← Headless CLI (LLR → Jama)
│   └── serve.sh
│
├── deployments/ hpc/ ci_cd/ dbscripts/ ← Infrastructure (extend as needed)
├── requirements.txt
├── pyproject.toml
├── .gitignore
├── .gitattributes
└── .gitlab-ci.yml
```

---

## Setup

### 1. Prerequisites

| Tool | Version | Install |
|------|---------|---------|
| Python | 3.10+ | [python.org](https://python.org) |
| GCC | any | `sudo apt-get install build-essential` |
| Ollama | latest | [ollama.com](https://ollama.com) |

### 2. Clone the repository

```bash
git clone <repo-url>
cd swreq
```

### 3. Create and activate a virtual environment

```bash
python -m venv venv
source venv/bin/activate       # Linux/Mac
venv\Scripts\activate          # Windows
```

### 4. Install Python dependencies

```bash
pip install -r requirements.txt
```

> **Note:** `torch` and `transformers` are **not required**. All LLM inference is handled by Ollama.

### 5. Set up `fake_libc_include` (required by pycparser)

```bash
git clone https://github.com/eliben/pycparser.git /tmp/pycparser
cp -r /tmp/pycparser/utils/fake_libc_include .
```

Or find your pycparser install and copy from there:
```bash
python -c "import pycparser; print(pycparser.__file__)"
# Then copy from <that_path>/utils/fake_libc_include to ./fake_libc_include
```

### 6. Pull an Ollama model

```bash
ollama serve &          # start Ollama in the background
ollama pull mistral:7b  # or llama3.2, qwen2.5, etc.
```

### 7. Add your C source files

```bash
cp /your/codebase/*.c  data/P3_MCP_Application/cmake-src/src/
```

---

## Running the Application

### One command — starts everything

```bash
bash run.sh
```

This will:
1. Check all dependencies and Ollama connectivity
2. Start the **FastAPI server** on port `8000`
3. Start the **Streamlit UI** on port `8501`
4. Print the URLs and log file locations

Then open: **http://localhost:8501**

### Other run modes

```bash
bash run.sh --check     # verify setup without starting servers
bash run.sh --api-only  # FastAPI only
bash run.sh --ui-only   # Streamlit only
bash run.sh --stop      # kill running servers
```

### Environment variable overrides

```bash
OLLAMA_MODEL=llama3.2 API_PORT=9000 UI_PORT=9001 bash run.sh
```

| Variable | Default | Description |
|----------|---------|-------------|
| `OLLAMA_BASE_URL` | `http://localhost:11434` | Ollama server URL |
| `OLLAMA_MODEL` | `mistral:7b` | Default model (overridable in UI too) |
| `API_HOST` | `0.0.0.0` | FastAPI bind address |
| `API_PORT` | `8000` | FastAPI port |
| `UI_PORT` | `8501` | Streamlit port |

---

## Using the UI

### Stage 1 — Code → LLR

1. Select a **C module** from the dropdown
2. Click **▶ Generate LLRs**
3. Review and edit each generated requirement inline
4. Click **Save Word Document** or **Save CSV** to export to `artifacts/`
5. Proceed to Stage 2 using the tab

### Stage 2 — LLR → Jama HLR

1. Choose **"Use Stage 1 output"** (auto-loaded) or upload a CSV
   - CSV must have columns: `Function Name`, `Requirements`
2. Click **▶ Generate Jama HLRs**
3. Review the generated HLRs in the preview table
4. Export as **Jama Excel (.xlsx)** or **HLR CSV** — saved to `artifacts/`

---

## API Reference

The FastAPI server exposes these endpoints (Swagger UI: `http://localhost:8000/docs`):

| Method | Path | Description |
|--------|------|-------------|
| `GET` | `/health` | Liveness + Ollama connectivity |
| `GET` | `/models` | List available Ollama models |
| `POST` | `/generate` | Stage 1: two-prompt code → LLR call |
| `POST` | `/rewrite` | Stage 2: LLR → Jama HLR rewrite |

---

## Outputs

All outputs are timestamped and saved to `artifacts/`:

| File | Stage | Description |
|------|-------|-------------|
| `YYYYMMDD_HHMMSS_<module>_LLR.docx` | 1 | Word document with LLRs |
| `YYYYMMDD_HHMMSS_<module>_LLR.csv` | 1 | CSV of function + requirement pairs |
| `YYYYMMDD_HHMMSS_<stem>_Jama_HLR.xlsx` | 2 | Jama-importable Excel workbook |
| `YYYYMMDD_HHMMSS_<stem>_Jama_HLR.csv` | 2 | HLR CSV with ID, description, type |
| `run_log.xlsx` | 1 | Cumulative run log (leaked syntax, missing globals) |
| `logs/api.log` | — | FastAPI server log |
| `logs/ui.log` | — | Streamlit UI log |

---

## Headless CLI (no UI)

For automated pipelines, run Stage 2 directly:

```bash
python main.py --input artifacts/my_LLRs.csv --model mistral:7b
# Output: artifacts/my_LLRs_jama.xlsx
```

---

## Testing

```bash
pytest                        # all tests
pytest tests/development/     # unit tests
pytest tests/sit/             # system integration
pytest tests/uat/             # user acceptance
pytest tests/production/      # smoke tests
```

---

## Switching Models

Any model pulled into Ollama works. In the UI, use the **Settings sidebar** to select the active model. Or set the environment variable:

```bash
ollama pull llama3.2
OLLAMA_MODEL=llama3.2 bash run.sh
```

Recommended models (good quality / speed tradeoff):

| Model | Size | Notes |
|-------|------|-------|
| `mistral:7b` | 4 GB | Default, fast, reliable |
| `llama3.2` | 2 GB | Smaller, good for quick iteration |
| `qwen2.5:14b` | 9 GB | Higher quality for complex requirements |
| `deepseek-r1:14b` | 9 GB | Strong reasoning |

