#!/usr/bin/env bash
# =============================================================================
# run.sh — Launch the entire swreq application
#
# Usage:
#   bash run.sh                 # start both FastAPI + Streamlit
#   bash run.sh --api-only      # start only the FastAPI server
#   bash run.sh --ui-only       # start only the Streamlit UI
#   bash run.sh --check         # verify dependencies and Ollama, then exit
#   bash run.sh --stop          # kill any running swreq processes
#
# Environment variables (override defaults):
#   OLLAMA_BASE_URL   URL of the Ollama server  (default: http://localhost:11434)
#   OLLAMA_MODEL      Model to use              (default: mistral:7b)
#   API_HOST          FastAPI bind address       (default: 0.0.0.0)
#   API_PORT          FastAPI port               (default: 8000)
#   UI_PORT           Streamlit port             (default: 8501)
# =============================================================================

set -euo pipefail

# ── Always run from the repo root ─────────────────────────────────────────────
# This ensures all relative paths (src/, data/, artifacts/) resolve correctly
# regardless of where the user invokes this script from.
cd "$(dirname "$(realpath "$0")")"

# ── Defaults ──────────────────────────────────────────────────────────────────
OLLAMA_BASE_URL="${OLLAMA_BASE_URL:-http://localhost:11434}"
OLLAMA_MODEL="${OLLAMA_MODEL:-mistral:7b}"
API_HOST="${API_HOST:-0.0.0.0}"
API_PORT="${API_PORT:-8000}"
UI_PORT="${UI_PORT:-8501}"

MODE="both"
if [[ "${1:-}" == "--api-only"  ]]; then MODE="api"; fi
if [[ "${1:-}" == "--ui-only"   ]]; then MODE="ui";  fi
if [[ "${1:-}" == "--check"     ]]; then MODE="check"; fi
if [[ "${1:-}" == "--stop"      ]]; then MODE="stop";  fi

LOG_DIR="artifacts/logs"
mkdir -p "$LOG_DIR"
API_LOG="$LOG_DIR/api.log"
UI_LOG="$LOG_DIR/ui.log"

# ── Colour helpers ────────────────────────────────────────────────────────────
GREEN="\033[0;32m"; YELLOW="\033[1;33m"; RED="\033[0;31m"; NC="\033[0m"
ok()   { echo -e "${GREEN}  ✔  $*${NC}"; }
warn() { echo -e "${YELLOW}  ⚠  $*${NC}"; }
err()  { echo -e "${RED}  ✗  $*${NC}"; }
hdr()  { echo -e "\n${GREEN}══ $* ══${NC}"; }

# ── Stop mode ─────────────────────────────────────────────────────────────────
if [[ "$MODE" == "stop" ]]; then
    hdr "Stopping swreq processes"
    pkill -f "uvicorn api.app:app"  2>/dev/null && ok "FastAPI stopped" || warn "FastAPI was not running"
    pkill -f "streamlit run src/llm_streamlit_app.py" 2>/dev/null && ok "Streamlit stopped" || warn "Streamlit was not running"
    exit 0
fi

# ── Dependency checks ─────────────────────────────────────────────────────────
hdr "Checking dependencies"

# Python
if ! command -v python3 &>/dev/null; then
    err "python3 not found. Install Python 3.10+."
    exit 1
fi
ok "Python $(python3 --version)"

# pip packages
for pkg in fastapi uvicorn streamlit requests pycparser pydantic openpyxl pandas regex yaml; do
    if python3 -c "import $pkg" 2>/dev/null; then
        ok "  $pkg"
    else
        err "  $pkg not installed. Run: pip install -r requirements.txt"
        exit 1
    fi
done

# GCC (needed by pycparser preprocessor)
if ! command -v gcc &>/dev/null; then
    warn "GCC not found. C preprocessing will fail. Install: sudo apt-get install build-essential"
else
    ok "GCC $(gcc --version | head -1)"
fi

# Ollama
hdr "Checking Ollama"
if ! command -v ollama &>/dev/null; then
    warn "Ollama CLI not found. Install from https://ollama.com"
else
    ok "Ollama CLI found"
fi

if curl -sf "${OLLAMA_BASE_URL}/api/tags" -o /dev/null 2>/dev/null; then
    ok "Ollama reachable at $OLLAMA_BASE_URL"

    # Check model
    if curl -sf "${OLLAMA_BASE_URL}/api/tags" | python3 -c \
        "import sys,json; models=[m['name'] for m in json.load(sys.stdin).get('models',[])]; \
         exit(0 if '$OLLAMA_MODEL' in models else 1)" 2>/dev/null; then
        ok "Model '$OLLAMA_MODEL' available"
    else
        warn "Model '$OLLAMA_MODEL' not found in Ollama."
        warn "Pull it with:  ollama pull $OLLAMA_MODEL"
    fi
else
    warn "Ollama not reachable at $OLLAMA_BASE_URL"
    warn "Start it with:  ollama serve"
fi

# Data directory
if compgen -G "data/P3_MCP_Application/cmake-src/src/*.c" > /dev/null 2>&1; then
    C_COUNT=$(ls data/P3_MCP_Application/cmake-src/src/*.c 2>/dev/null | wc -l)
    ok "$C_COUNT C source file(s) in data/P3_MCP_Application/cmake-src/src/"
else
    warn "No .c files found in data/P3_MCP_Application/cmake-src/src/"
    warn "Copy your C codebase there before running Stage 1."
fi

if [[ "$MODE" == "check" ]]; then
    hdr "Check complete — not starting servers"
    exit 0
fi

# ── Start FastAPI ─────────────────────────────────────────────────────────────
start_api() {
    hdr "Starting FastAPI server (port $API_PORT)"
    export OLLAMA_BASE_URL OLLAMA_MODEL
    uvicorn api.app:app \
        --host "$API_HOST" \
        --port "$API_PORT" \
        --log-level info \
        > "$API_LOG" 2>&1 &
    API_PID=$!
    echo "$API_PID" > "$LOG_DIR/api.pid"

    # Wait for API to be ready (up to 15 s)
    for i in $(seq 1 15); do
        sleep 1
        if curl -sf "http://localhost:${API_PORT}/health" -o /dev/null 2>/dev/null; then
            ok "FastAPI ready  →  http://localhost:${API_PORT}"
            ok "Swagger docs   →  http://localhost:${API_PORT}/docs"
            return 0
        fi
    done
    err "FastAPI did not become ready in time. Check $API_LOG"
    return 1
}

# ── Start Streamlit ───────────────────────────────────────────────────────────
start_ui() {
    hdr "Starting Streamlit UI (port $UI_PORT)"
    streamlit run src/llm_streamlit_app.py \
        --server.port "$UI_PORT" \
        --server.headless true \
        --browser.gatherUsageStats false \
        > "$UI_LOG" 2>&1 &
    UI_PID=$!
    echo "$UI_PID" > "$LOG_DIR/ui.pid"

    for i in $(seq 1 15); do
        sleep 1
        if curl -sf "http://localhost:${UI_PORT}/_stcore/health" -o /dev/null 2>/dev/null; then
            ok "Streamlit ready  →  http://localhost:${UI_PORT}"
            return 0
        fi
    done
    err "Streamlit did not become ready in time. Check $UI_LOG"
    return 1
}

# ── Launch ────────────────────────────────────────────────────────────────────
case "$MODE" in
    api)  start_api ;;
    ui)   start_ui  ;;
    both)
        start_api
        start_ui
        hdr "swreq is running"
        echo ""
        echo -e "  ${GREEN}Streamlit UI   →  http://localhost:${UI_PORT}${NC}"
        echo -e "  ${GREEN}FastAPI docs   →  http://localhost:${API_PORT}/docs${NC}"
        echo ""
        echo "  Logs:  $API_LOG  |  $UI_LOG"
        echo "  Stop:  bash run.sh --stop"
        echo ""

        # Keep script alive and relay Ctrl-C to children
        trap 'bash run.sh --stop; exit 0' INT TERM
        wait
        ;;
esac
