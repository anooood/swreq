#!/usr/bin/env bash
# serve.sh — Start the FastAPI LLM server
set -euo pipefail

HOST="${HOST:-0.0.0.0}"
PORT="${PORT:-8000}"

echo "Starting LLM server on ${HOST}:${PORT}..."
uvicorn api.app:app --host "$HOST" --port "$PORT" --reload
