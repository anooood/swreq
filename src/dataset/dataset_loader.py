"""
dataset_loader.py
-----------------
Loads input datasets (CSV files containing LLRs or draft HLRs) for processing.
Extend this module to support additional data sources or formats.
"""

from __future__ import annotations
from pathlib import Path
import pandas as pd


def load_csv(filepath: str | Path) -> pd.DataFrame:
    """Load a CSV file and return a DataFrame."""
    return pd.read_csv(filepath)
