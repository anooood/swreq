"""
core/database.py
-----------------
SQLite-backed user store with bcrypt password hashing.

Tables:
  users          — username (unique) + bcrypt password hash
  user_sessions  — serialised Streamlit session state per user

DB file is stored at <repo_root>/users.db by default; override with the
SWREQ_DB_PATH environment variable for Docker volume mounts.
"""

from __future__ import annotations

import os
import pickle
import sqlite3
from datetime import datetime
from pathlib import Path

import bcrypt

_REPO_ROOT = Path(__file__).resolve().parent.parent
DB_PATH = os.getenv("SWREQ_DB_PATH", str(_REPO_ROOT / "users.db"))


def _connect():
    return sqlite3.connect(DB_PATH)


# ───────────────────────────────────────────────────────────────────────────
# Schema
# ───────────────────────────────────────────────────────────────────────────

def init_db() -> None:
    """Create tables if they don't exist."""
    with _connect() as conn:
        c = conn.cursor()
        c.execute("""
            CREATE TABLE IF NOT EXISTS users (
                id            INTEGER PRIMARY KEY AUTOINCREMENT,
                username      TEXT UNIQUE NOT NULL,
                password_hash TEXT NOT NULL,
                created_at    TIMESTAMP NOT NULL
            )
        """)
        c.execute("""
            CREATE TABLE IF NOT EXISTS user_sessions (
                user_id      INTEGER PRIMARY KEY,
                session_data BLOB NOT NULL,
                updated_at   TIMESTAMP NOT NULL,
                FOREIGN KEY (user_id) REFERENCES users(id)
            )
        """)
        conn.commit()


# ───────────────────────────────────────────────────────────────────────────
# User CRUD
# ───────────────────────────────────────────────────────────────────────────

def add_user(username: str, password: str) -> bool:
    """Create a new user. Returns False if the username already exists."""
    pw_hash = bcrypt.hashpw(password.encode("utf-8"), bcrypt.gensalt()).decode("utf-8")
    with _connect() as conn:
        c = conn.cursor()
        try:
            c.execute(
                "INSERT INTO users (username, password_hash, created_at) VALUES (?, ?, ?)",
                (username, pw_hash, datetime.utcnow()),
            )
            conn.commit()
            return True
        except sqlite3.IntegrityError:
            return False


def verify_user(username: str, password: str) -> bool:
    """Return True if the username exists and the password matches."""
    with _connect() as conn:
        c = conn.cursor()
        c.execute("SELECT password_hash FROM users WHERE username = ?", (username,))
        row = c.fetchone()
    if not row:
        return False
    return bcrypt.checkpw(password.encode("utf-8"), row[0].encode("utf-8"))


def get_user_id(username: str) -> int | None:
    with _connect() as conn:
        c = conn.cursor()
        c.execute("SELECT id FROM users WHERE username = ?", (username,))
        row = c.fetchone()
    return row[0] if row else None


def list_users() -> list[str]:
    with _connect() as conn:
        c = conn.cursor()
        c.execute("SELECT username FROM users ORDER BY username")
        return [r[0] for r in c.fetchall()]


def delete_user(username: str) -> bool:
    with _connect() as conn:
        c = conn.cursor()
        c.execute("DELETE FROM users WHERE username = ?", (username,))
        conn.commit()
        return c.rowcount > 0


def change_password(username: str, new_password: str) -> bool:
    pw_hash = bcrypt.hashpw(new_password.encode("utf-8"), bcrypt.gensalt()).decode("utf-8")
    with _connect() as conn:
        c = conn.cursor()
        c.execute("UPDATE users SET password_hash = ? WHERE username = ?", (pw_hash, username))
        conn.commit()
        return c.rowcount > 0


# ───────────────────────────────────────────────────────────────────────────
# Session persistence (optional — stores Streamlit state across logins)
# ───────────────────────────────────────────────────────────────────────────

def save_session(user_id: int, data: dict) -> None:
    with _connect() as conn:
        c = conn.cursor()
        c.execute(
            "INSERT OR REPLACE INTO user_sessions (user_id, session_data, updated_at) VALUES (?, ?, ?)",
            (user_id, pickle.dumps(data), datetime.utcnow()),
        )
        conn.commit()


def load_session(user_id: int) -> dict | None:
    with _connect() as conn:
        c = conn.cursor()
        c.execute("SELECT session_data FROM user_sessions WHERE user_id = ?", (user_id,))
        row = c.fetchone()
    return pickle.loads(row[0]) if row else None


def delete_session(user_id: int) -> None:
    with _connect() as conn:
        c = conn.cursor()
        c.execute("DELETE FROM user_sessions WHERE user_id = ?", (user_id,))
        conn.commit()
