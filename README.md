# swreq — Software Requirements Generator

Parses C source code and produces formal, **Jama-compliant Low-Level Requirements (LLR)** through a fully automated two-stage LLM pipeline.

All LLM inference is handled by a **local Ollama instance** — no cloud API keys, no model weights in the repo.

The recommended way to run swreq is in **Docker**. The same `docker compose` commands work identically on Linux, macOS, and Windows (WSL 2 or Docker Desktop).

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
  Jama-compliant LLRs (ID, shall statement, verification, type)
      │
      ▼
  Jama-importable Excel .xlsx + LLR .csv
```

Both stages run inside a single **Streamlit UI**. LLM calls are routed through a **FastAPI server** that acts as a thin proxy to Ollama.

---

## Project Structure

```
swreq/
├── api/
│   ├── app.py                          ← FastAPI server (Ollama proxy)
│   └── auth.py                         ← Bearer-token auth dependency
│
├── core/
│   ├── auth.py                         ← Streamlit login + token issuer
│   └── database.py                     ← SQLite + bcrypt user store
│
├── src/
│   ├── llm_streamlit_app.py            ← Unified Streamlit UI (Stage 1 + 2)
│   ├── dataset/
│   │   └── preprocess.py               ← C parser, function extractor (pycparser)
│   ├── pipelines/
│   │   ├── inference_pipeline.py       ← LLR → Jama LLR rewrite pipeline
│   │   ├── exporter.py                 ← MergeResult → Jama Excel workbook
│   │   └── hierarchy_sheet.py          ← Jama hierarchy sheet builder
│   └── utils/
│       ├── prompt_templates.py         ← All LLM prompt templates
│       ├── prompt_logger.py
│       ├── reference_loader.py
│       └── word_output.py              ← Word document generator
│
├── configs/
│   └── model.yaml                      ← Stage 1 + Stage 2 model config
│
├── data/
│   ├── references/                     ← Verification methods + requirement type docs
│   └── fake_libc_include/              ← pycparser stub headers (required)
│
├── deployments/
│   ├── docker/
│   │   ├── Dockerfile.api              ← FastAPI image
│   │   └── Dockerfile.ui               ← Streamlit image
│   ├── edge_device/
│   └── k8s/
│
├── notebooks/                          ← Jupyter experiments only
├── scripts/                            ← CLI helpers
├── tests/                              ← development / sit / uat / production
├── artifacts/                          ← All generated outputs land here
│   └── logs/
│
├── ci_cd/  hpc/  dbscripts/            ← Infrastructure (extend as needed)
│
├── docker-compose.yml
├── .env.example
├── manage_users.py                     ← User-management CLI
├── run.sh                              ← Bare-metal launcher (alternative to Docker)
├── requirements.txt
├── pyproject.toml
├── .gitignore  .gitattributes  .gitlab-ci.yml
└── README.md
```

---

## Prerequisites

These are the only host-side requirements. Everything else runs inside containers.

| Tool | Version | Notes |
|------|---------|-------|
| Docker Engine + Docker Compose v2 | 24+ | macOS/Windows: install [Docker Desktop](https://www.docker.com/products/docker-desktop/). Linux: install Docker Engine + the `docker compose` plugin. |
| Ollama | latest | Install from [ollama.com](https://ollama.com). Runs on the **host**, not in a container. |

> **Why is Ollama not in Docker?** Ollama needs direct GPU access for acceptable performance. Running it on the host (with the GPU drivers it already knows about) is simpler and faster than wiring NVIDIA/Metal passthrough through Compose. The containers reach it over `host.docker.internal`.

### Pull the models you plan to use

In a host terminal — once Ollama is installed and `ollama serve` is running (Docker Desktop / the Ollama installer typically starts it for you):

```bash
ollama pull qwen3-coder:30b      # Stage 1 default
ollama pull qwen2.5:latest       # Stage 2 default
```

Smaller alternatives if VRAM is tight: `mistral:7b`, `llama3.2`, `qwen2.5:14b`.

---

## Quick Start (Docker, all OSes)

### 1. Clone the repository

```bash
git clone <repo-url>
cd swreq
```

### 2. Create your environment file

Copy the template and fill it in. Use whichever copy command your shell understands:

```bash
# Linux / macOS / Git Bash / WSL
cp .env.example .env
```

```powershell
# Windows PowerShell
Copy-Item .env.example .env
```

Open `.env` in any editor and at minimum set:

| Variable | What to set it to |
|----------|-------------------|
| `SWREQ_API_SECRET` | A long random string. Generate one with `openssl rand -hex 32` (Linux/macOS/WSL/Git Bash) or any password generator. |
| `OLLAMA_BASE_URL` | Leave as `http://host.docker.internal:11434` — works on Linux, macOS, and Windows thanks to the `extra_hosts` entry in `docker-compose.yml`. |
| `STAGE1_MODEL` / `STAGE2_MODEL` | Match whatever you `ollama pull`-ed. |

### 3. Build and start the stack

```bash
docker compose up --build -d
```

This builds two images (`swreq-api`, `swreq-ui`) and starts both containers in the background. The API healthcheck must pass before the UI starts, so first boot takes ~30 s.

### 4. Initialise the user database and create your first account

```bash
docker compose exec api python manage_users.py init
docker compose exec api python manage_users.py add
```

You'll be prompted for a username (e.g. `firstname.lastname`) and a password (8+ characters).

### 5. Open the app

Browse to **http://localhost:8501** and log in.

The FastAPI Swagger docs are at **http://localhost:8000/docs**.

---

## Day-to-day Docker Commands

| Task | Command |
|------|---------|
| Start (already built) | `docker compose up -d` |
| Stop | `docker compose down` |
| Rebuild after code changes | `docker compose up --build -d` |
| Tail API logs | `docker compose logs -f api` |
| Tail UI logs | `docker compose logs -f ui` |
| Open a shell in the API container | `docker compose exec api bash` |
| List users | `docker compose exec api python manage_users.py list` |
| Reset a user's password | `docker compose exec api python manage_users.py passwd <username>` |
| Delete a user | `docker compose exec api python manage_users.py delete <username>` |

### What's mounted from the host

`docker-compose.yml` mounts two host directories as volumes so your data survives container rebuilds:

| Host path | Container path | Purpose |
|-----------|----------------|---------|
| `./data` | `/app/data` | `users.db`, uploaded codebases, parsed `.i` files |
| `./artifacts` | `/app/artifacts` | All generated outputs (`.docx`, `.csv`, `.xlsx`) and logs |

Outputs you generate in the UI appear in `./artifacts/` on your host immediately.

---

## Linux-specific note: file permissions

On Linux the bind-mounted `./data` and `./artifacts` directories will be owned by whatever UID the container writes as. By default that's UID `1000`, which matches most single-user Linux systems. If your host UID differs, build with overrides:

```bash
HOST_UID=$(id -u) HOST_GID=$(id -g) docker compose up --build -d
```

macOS and Windows users do **not** need this — Docker Desktop handles the UID translation transparently.

---

## Switching Models

Edit `STAGE1_MODEL` and/or `STAGE2_MODEL` in your `.env`, then:

```bash
docker compose up -d
```

(No rebuild needed — these are environment variables.)

Make sure the new model is pulled into Ollama on the host first:

```bash
ollama pull <model-name>
```

Recommended models:

| Model | Size | Notes |
|-------|------|-------|
| `qwen3-coder:30b` | ~17 GB | Stage 1 default. Strongest code comprehension. |
| `qwen2.5:latest` | ~5 GB | Stage 2 default. Reliable JSON output. |
| `mistral:7b` | ~4 GB | Lower-VRAM Stage 1 alternative. |
| `llama3.2` | ~2 GB | Quick iteration / smoke testing. |
| `deepseek-r1:14b` | ~9 GB | Strong reasoning, slower. |

---

## Using the UI

### Stage 1 — Code → LLR

1. **Upload Codebase (ZIP)** — the app auto-detects the folder containing the most `.c` files.
2. **Select a C module** from the dropdown.
3. Click **▶ Generate LLRs**.
4. Review and edit each generated requirement inline; tick **User approved** on every requirement.
5. Click **Apply changes** for each one, then **Save Word Document** or **Save CSV**.

### Stage 2 — LLR → Jama LLR

1. Choose **"Use Stage 1 output"** (auto-loaded once all Stage 1 requirements are approved) **or** upload an LLR CSV with columns `Function Name` and `Requirements`.
2. Click **▶ Generate Jama LLRs**.
3. Export as **Jama Excel (.xlsx)** or **HLR CSV**.

All exports land in `./artifacts/` with a timestamp prefix.

---

## API Reference

Swagger UI: `http://localhost:8000/docs`

| Method | Path | Description |
|--------|------|-------------|
| `GET` | `/health` | Liveness + Ollama connectivity |
| `GET` | `/models` | List available Ollama models |
| `GET` | `/config` | Show active model configuration |
| `POST` | `/generate` | Stage 1: two-prompt code → LLR call |
| `POST` | `/rewrite` | Stage 2: LLR → Jama LLR rewrite |
| `POST` | `/reset_stage1` | Evict & reload the Stage 1 model |

All endpoints except `/health`, `/models`, and `/config` require a Bearer token issued by the Streamlit UI on login.

---

## Outputs

All outputs are timestamped and saved under `./artifacts/`:

| File | Stage | Description |
|------|-------|-------------|
| `YYYYMMDD_HHMMSS_<module>_LLR.docx` | 1 | Word document with LLRs |
| `YYYYMMDD_HHMMSS_<module>_LLR.csv` | 1 | CSV of function + requirement pairs |
| `YYYYMMDD_HHMMSS_<stem>_Jama_LLR.xlsx` | 2 | Jama-importable Excel workbook |
| `YYYYMMDD_HHMMSS_<stem>_Jama_LLR.csv` | 2 | LLR CSV with ID, description, type |
| `run_log.xlsx` | 1 | Cumulative run log (leaked syntax, missing globals) |
| `logs/api.log`, `logs/ui.log` | — | Server logs |

---

## Running Without Docker (bare metal)

If you'd rather not use Docker, the legacy launcher still works on Linux/macOS/WSL.

```bash
python -m venv venv
source venv/bin/activate                  # Windows: venv\Scripts\activate
pip install -r requirements.txt
ollama serve &                            # if not already running
ollama pull qwen3-coder:30b
bash run.sh                               # starts API on 8000 + UI on 8501
```

`run.sh` flags: `--check`, `--api-only`, `--ui-only`, `--stop`.

`pycparser` needs `data/fake_libc_include/` — it's already in the repo, so no extra setup is required.

---

## Headless CLI (no UI)

For automated pipelines, run Stage 2 directly inside the API container:

```bash
docker compose exec api python scripts/main.py \
    --input artifacts/my_LLRs.csv \
    --model qwen2.5:latest
# Output: artifacts/my_LLRs_jama.xlsx
```

---

## Testing

```bash
docker compose exec api pytest                     # all tests
docker compose exec api pytest tests/development/  # unit tests
docker compose exec api pytest tests/sit/          # system integration
docker compose exec api pytest tests/uat/          # user acceptance
docker compose exec api pytest tests/production/   # smoke tests
```

---

## Troubleshooting

| Symptom | Fix |
|---------|-----|
| UI says "API ❌" | `docker compose logs api` — usually means Ollama isn't running on the host. Start it with `ollama serve`. |
| "Cannot reach Ollama at host.docker.internal:11434" | Confirm `ollama serve` is running on the host, then `curl http://localhost:11434/api/tags` from the host. On Linux without Docker Desktop, the `extra_hosts: host.docker.internal:host-gateway` line in `docker-compose.yml` provides this name. |
| 401 Unauthorized from the API | Your `SWREQ_API_SECRET` differs between containers, or you forgot to log in again after restarting. Make sure it's set once in `.env` and rebuild. |
| First Stage 1 run fine, later runs degrade | Expected on long-lived servers — the UI calls `POST /reset_stage1` before every batch to flush Ollama's KV cache. If you see this anyway, check that `/reset_stage1` is reachable in the API logs. |
| `users.db` permission errors on Linux | Rebuild with `HOST_UID=$(id -u) HOST_GID=$(id -g) docker compose up --build -d`. |
