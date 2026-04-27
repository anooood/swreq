"""
exporter.py
-----------
Converts a MergeResult into a Jama-import-compatible Excel workbook.

The workbook now contains a SINGLE sheet:

  Jama_Hierarchy   — indented tree for Jama hierarchical import

The previous Jama_Requirements (flat list) and Jama_Hierarchy_Color
(color-coded per-level) sheets have been removed at the user's request:
the hierarchy sheet alone is what's actually used for Jama import.
"""

from __future__ import annotations

from typing import TYPE_CHECKING

from openpyxl import Workbook

if TYPE_CHECKING:
    from src.pipelines.inference_pipeline import MergeResult


# ---------------------------------------------------------------------------
# Public API
# ---------------------------------------------------------------------------

def export_to_excel(
    result: "MergeResult",
    output_path: str,
    filename,
) -> None:
    """
    Write the Jama Excel workbook to output_path.

    The workbook contains a single sheet, "Jama_Hierarchy", built by
    src.pipelines.hierarchy_sheet.build_hierarchy_sheet().

    Parameters
    ----------
    result      : MergeResult produced by the inference pipeline.
    output_path : Destination .xlsx file path.
    filename    : Stem used to build the document title for the hierarchy
                  root node (e.g. "Wind" → "Software Requirements for Wind Module").
    """
    from src.pipelines.hierarchy_sheet import build_hierarchy_sheet

    filename = filename.split('_')[0]
    module_name: str = f"Software Requirements for {filename} Module"

    wb = Workbook()

    # Remove the default empty "Sheet" that openpyxl creates with new
    # workbooks — we want the workbook to contain ONLY Jama_Hierarchy.
    default_sheet = wb.active
    wb.remove(default_sheet)

    build_hierarchy_sheet(wb, result, module_name=module_name)

    wb.save(output_path)
