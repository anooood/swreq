"""
api/auth.py
------------
FastAPI auth dependency. Verifies the Bearer token issued by the
Streamlit UI via core.auth.issue_api_token().

Usage in api/app.py:

    from fastapi import Depends
    from api.auth import require_user

    @app.post("/generate", dependencies=[Depends(require_user)])
    def generate(...):
        ...

Or, if you want to know *which* user called the endpoint:

    @app.post("/generate")
    def generate(req: GenerateRequest, user: str = Depends(require_user)):
        logger.info("generate called by %s", user)
        ...
"""

from __future__ import annotations

import sys
from pathlib import Path

from fastapi import Depends, HTTPException, status
from fastapi.security import HTTPAuthorizationCredentials, HTTPBearer

# Make `core.auth` importable when this file is loaded from api/
_REPO_ROOT = Path(__file__).resolve().parent.parent
if str(_REPO_ROOT) not in sys.path:
    sys.path.insert(0, str(_REPO_ROOT))

from core.auth import verify_api_token

_bearer = HTTPBearer(auto_error=False)


def require_user(creds: HTTPAuthorizationCredentials | None = Depends(_bearer)) -> str:
    """
    Dependency that validates the Authorization: Bearer <token> header
    and returns the username it was issued for.

    Raises 401 if missing/invalid/expired.
    """
    if creds is None or not creds.credentials:
        raise HTTPException(
            status_code=status.HTTP_401_UNAUTHORIZED,
            detail="Missing Authorization header",
            headers={"WWW-Authenticate": "Bearer"},
        )

    username = verify_api_token(creds.credentials)
    if username is None:
        raise HTTPException(
            status_code=status.HTTP_401_UNAUTHORIZED,
            detail="Invalid or expired token",
            headers={"WWW-Authenticate": "Bearer"},
        )
    return username
