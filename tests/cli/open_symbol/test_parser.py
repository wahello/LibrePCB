#!/usr/bin/env python
# -*- coding: utf-8 -*-

from helpers import strip_image_file_extensions

"""
Test command "open-symbol" (basic parser tests)
"""

HELP_TEXT = """\
Usage: {executable} [options] open-symbol [command_options] symbol
LibrePCB Command Line Interface

Options:
  -h, --help       Print this message.
  -V, --version    Displays version information.
  -v, --verbose    Verbose output.
  --check          Run the symbol check, print all non-approved messages and
                   report failure (exit code = 1) if there are non-approved
                   messages.
  --export <file>  Export the symbol to a graphical file. Supported file
                   extensions: pdf, svg, ***

Arguments:
  open-symbol      Open a symbol to execute symbol-related tasks.
  symbol           Path to symbol directory (containing *.lp).
"""

ERROR_TEXT = """\
{error}
Usage: {executable} [options] open-symbol [command_options] symbol
Help: {executable} open-symbol --help
"""


def test_help(cli):
    code, stdout, stderr = cli.run("open-symbol", "--help")
    assert stderr == ""
    assert strip_image_file_extensions(stdout) == HELP_TEXT.format(
        executable=cli.executable
    )
    assert code == 0


def test_no_arguments(cli):
    code, stdout, stderr = cli.run("open-symbol")
    assert stderr == ERROR_TEXT.format(
        executable=cli.executable,
        error="Missing arguments: symbol",
    )
    assert stdout == ""
    assert code == 1


def test_invalid_argument(cli):
    code, stdout, stderr = cli.run("open-symbol", "--invalid-argument")
    assert stderr == ERROR_TEXT.format(
        executable=cli.executable,
        error="Unknown option 'invalid-argument'.",
    )
    assert stdout == ""
    assert code == 1
