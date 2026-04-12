"""
config_utils.py
---------------
YAML configuration loading helpers.
"""

from __future__ import annotations
from pathlib import Path
import yaml


def load_config(path: str | Path) -> dict:
    """Load a YAML config file and return it as a dict."""
    with open(path, "r") as f:
        return yaml.safe_load(f)
