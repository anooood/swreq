"""
core/auth.py
-------------
Streamlit login form + auth gating for the Streamlit UI.

Usage in notebooks/llm_streamlit_app.py:

    from core.auth import require_login
    user = require_login()                  # blocks until authenticated
    # ... rest of the app ...

Also exposes `issue_api_token()` used by the UI to authenticate its
downstream calls to the FastAPI server.
"""

from __future__ import annotations

import hmac
import hashlib
import os
import time

import streamlit as st

from core.database import init_db, verify_user, get_user_id


# Shared secret used to sign short-lived API tokens. In production, set
# SWREQ_API_SECRET to a long random value in the environment of BOTH the
# Streamlit container and the FastAPI container.
_API_SECRET = os.getenv("SWREQ_API_SECRET", "change-me-in-production")
_TOKEN_TTL_S = 60 * 60 * 8   # 8 hours


def _sign(username: str, expiry: int) -> str:
    msg = f"{username}:{expiry}".encode("utf-8")
    sig = hmac.new(_API_SECRET.encode("utf-8"), msg, hashlib.sha256).hexdigest()
    return f"{username}:{expiry}:{sig}"


def issue_api_token(username: str) -> str:
    """Create an HMAC-signed token for the Streamlit UI to call the API."""
    expiry = int(time.time()) + _TOKEN_TTL_S
    return _sign(username, expiry)


def verify_api_token(token: str) -> str | None:
    """Verify a token and return the username it was issued for, or None."""
    try:
        username, expiry_str, sig = token.rsplit(":", 2)
        expiry = int(expiry_str)
    except (ValueError, AttributeError):
        return None
    if expiry < int(time.time()):
        return None
    expected = _sign(username, expiry).rsplit(":", 1)[-1]
    return username if hmac.compare_digest(sig, expected) else None


def require_login() -> str:
    """
    Render the login form and block until the user is authenticated.
    Returns the logged-in username.
    """
    init_db()

    if st.session_state.get("authenticated"):
        return st.session_state["username"]

    st.title("🔐 Sign in")
    st.caption("Contact your administrator if you don't have an account.")

    with st.form("login_form"):
        username = st.text_input("Username", key="_login_username").strip()
        password = st.text_input("Password", type="password", key="_login_password")
        submitted = st.form_submit_button("Sign in", type="primary")

    if submitted:
        if verify_user(username, password):
            st.session_state["authenticated"] = True
            st.session_state["username"]      = username
            st.session_state["user_id"]       = get_user_id(username)
            st.session_state["api_token"]     = issue_api_token(username)
            st.rerun()
        else:
            st.error("Invalid username or password")

    st.stop()  # nothing below runs until authenticated


def logout_button() -> None:
    """Render a Logout button in the sidebar."""
    with st.sidebar:
        st.markdown(f"👤 **{st.session_state.get('username', 'unknown')}**")
        if st.button("Log out"):
            for k in ("authenticated", "username", "user_id", "api_token"):
                st.session_state.pop(k, None)
            st.rerun()
