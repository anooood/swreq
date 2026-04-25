#!/usr/bin/env python3
"""
manage_users.py
----------------
CLI for managing swreq user accounts.

Usage:
    python manage_users.py init                    # create the SQLite schema
    python manage_users.py add                     # interactive: add a user
    python manage_users.py list                    # list all usernames
    python manage_users.py delete <username>       # remove a user
    python manage_users.py passwd <username>       # reset a user's password

The DB file lives at $SWREQ_DB_PATH (default: <repo>/users.db).
In Docker, mount a volume at /app/data/users.db and set SWREQ_DB_PATH.
"""

import argparse
import getpass
import sys

from core.database import (
    add_user,
    change_password,
    delete_user,
    init_db,
    list_users,
)


def _prompt_password(label: str = "Password") -> str | None:
    pw  = getpass.getpass(f"{label}: ")
    pw2 = getpass.getpass(f"Confirm {label.lower()}: ")
    if pw != pw2:
        print("❌ Passwords do not match.")
        return None
    if len(pw) < 8:
        print("❌ Password must be at least 8 characters.")
        return None
    return pw


def main() -> int:
    parser = argparse.ArgumentParser(description="swreq user management")
    sub = parser.add_subparsers(dest="command", required=True)

    sub.add_parser("init",   help="Initialise the user database")
    sub.add_parser("add",    help="Add a new user")
    sub.add_parser("list",   help="List all users")

    p_del = sub.add_parser("delete", help="Delete a user")
    p_del.add_argument("username")

    p_pw  = sub.add_parser("passwd", help="Reset a user's password")
    p_pw.add_argument("username")

    args = parser.parse_args()
    init_db()  # safe to call on every command

    if args.command == "init":
        print("✅ Database initialised.")

    elif args.command == "add":
        username = input("Username (e.g. firstname.lastname): ").strip()
        if not username:
            print("❌ Username cannot be empty.")
            return 1
        pw = _prompt_password()
        if pw is None:
            return 1
        if add_user(username, pw):
            print(f"✅ User '{username}' added.")
        else:
            print(f"❌ User '{username}' already exists.")
            return 1

    elif args.command == "list":
        users = list_users()
        if not users:
            print("(no users)")
        else:
            print(f"{len(users)} user(s):")
            for u in users:
                print(f"  • {u}")

    elif args.command == "delete":
        if delete_user(args.username):
            print(f"✅ User '{args.username}' deleted.")
        else:
            print(f"❌ User '{args.username}' not found.")
            return 1

    elif args.command == "passwd":
        pw = _prompt_password("New password")
        if pw is None:
            return 1
        if change_password(args.username, pw):
            print(f"✅ Password updated for '{args.username}'.")
        else:
            print(f"❌ User '{args.username}' not found.")
            return 1

    return 0


if __name__ == "__main__":
    sys.exit(main())
