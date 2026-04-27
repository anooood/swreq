"""
core/auth.py
-------------
Streamlit login form + auth gating for the Streamlit UI.

Usage in src/llm_streamlit_app.py:

    from core.auth import require_login, sidebar_user_info
    user = require_login()                  # blocks until authenticated
    sidebar_user_info()                     # render user + logout at top of sidebar
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
    Render the login form (centered, fixed-width) and block until the
    user is authenticated. Returns the logged-in username.
    """
    init_db()

    if st.session_state.get("authenticated"):
        return st.session_state["username"]

    # Center the login form using a 1:2:1 column layout so it doesn't
    # stretch to fill the wide page on `layout="wide"`.
    _left, center, _right = st.columns([1, 2, 1])
    with center:
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


def sidebar_user_info() -> None:
    """
    Render user info and a Logout button at the TOP of the sidebar,
    followed by a divider so the rest of the sidebar content sits
    below it cleanly.

    Call this once, before any other sidebar content.
    """
    with st.sidebar:
        col_user, col_btn = st.columns([2, 1], vertical_alignment="center")
        with col_user:
            st.markdown(
                f"👤 **{st.session_state.get('username', 'unknown')}**"
            )
        with col_btn:
            if st.button("Log out", key="_logout_btn", use_container_width=True):
                for k in ("authenticated", "username", "user_id", "api_token"):
                    st.session_state.pop(k, None)
                st.rerun()
        st.divider()


# ── Legacy aliases kept for backward compatibility ──────────────────────────

def logout_button() -> None:
    """Deprecated — use sidebar_user_info() instead."""
    sidebar_user_info()


def top_bar() -> None:
    """Deprecated — use sidebar_user_info() instead."""
    sidebar_user_info()
