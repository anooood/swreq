# ─────────────────────────────────────────────────────────────────────────
# Dockerfile — unified swreq image (API + UI)
#
# Two build targets share the same base layer:
#   --target api  → FastAPI server on :8000
#   --target ui   → Streamlit UI on :8501
#
# Compose selects the target via `build.target` in docker-compose.yml.
# ─────────────────────────────────────────────────────────────────────────

# ─── Stage 1: shared base ────────────────────────────────────────────────
FROM python:3.11-slim AS base

ENV PYTHONDONTWRITEBYTECODE=1 \
    PYTHONUNBUFFERED=1 \
    PIP_NO_CACHE_DIR=1

# gcc        — required by pycparser for C preprocessing
# libffi/ssl — required by bcrypt on slim images
RUN apt-get update && apt-get install -y --no-install-recommends \
        build-essential gcc \
        libffi-dev libssl-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

# Copy application code (shared by both services)
COPY api/                    ./api/
COPY core/                   ./core/
COPY src/                    ./src/
COPY configs/                ./configs/
COPY .streamlit/             ./.streamlit/
COPY data/references/        ./data/references/
COPY data/fake_libc_include/ ./data/fake_libc_include/
COPY manage_users.py         ./

# Create runtime directories and a non-root user matching the host UID/GID
ARG HOST_UID=1000
ARG HOST_GID=1000
RUN mkdir -p /app/artifacts/logs /app/data /app/data/C_Parsed && \
    groupadd -g ${HOST_GID} swreq && \
    useradd -u ${HOST_UID} -g ${HOST_GID} -m -s /bin/bash swreq && \
    chown -R swreq:swreq /app

USER swreq

# ─── Stage 2a: API ───────────────────────────────────────────────────────
FROM base AS api

EXPOSE 8000
CMD ["uvicorn", "api.app:app", "--host", "0.0.0.0", "--port", "8000"]

# ─── Stage 2b: UI ────────────────────────────────────────────────────────
FROM base AS ui

EXPOSE 8501
CMD ["streamlit", "run", "src/llm_streamlit_app.py", \
     "--server.port", "8501", \
     "--server.address", "0.0.0.0", \
     "--server.headless", "true", \
     "--browser.gatherUsageStats", "false"]
