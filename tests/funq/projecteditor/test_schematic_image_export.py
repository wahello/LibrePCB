#!/usr/bin/env python
# -*- coding: utf-8 -*-

import pytest
import os

"""
Test image export dialog in schematic editor

Meant as an addition to the tests in test_schematic_pdf_export.py just to
test the image export functionality as well.
"""


@pytest.fixture
def dialog(project_editor):
    """
    Fixture opening the image export dialog in the schematic editor
    """
    adapter = project_editor.widget("mainWindowTestAdapter", wait_active=True)
    adapter.call_slot("trigger", "schematic-export-image-dialog")
    dialog = project_editor.widget("graphicsExportDialog")

    # Do not open exported files since this would be annoying here.
    project_editor.widget("graphicsExportDialogOpenFileCheckbox").set_property(
        "checked", False
    )

    yield dialog


def test_export_png(dialog, project_editor, librepcb, helpers):
    """
    Test if the accept button asks for a filepath and then creates a PNG file
    """
    project_editor.widget("graphicsExportDialogButtonAccept").click()
    path = librepcb.abspath("test export.png")
    project_editor.widget("graphicsExportSaveFileDialogFileNameEdit").set_property(
        "text", path
    )
    project_editor.widget("graphicsExportSaveFileDialogOkButton").click()
    helpers.wait_until_widget_hidden(dialog)  # raises on timeout
    assert os.path.exists(path)
