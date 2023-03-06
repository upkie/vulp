# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("@rules_python//python:pip.bzl", "pip_parse")

def parse_deps():
    """
    Parse Python dependencies to a @pip_vulp external repository.

    This function intended to be loaded and called from your WORKSPACE.
    """
    pip_parse(
        name = "pip_vulp",
        requirements_lock = Label("//tools/workspace/pip_vulp:requirements_lock.txt"),
    )
