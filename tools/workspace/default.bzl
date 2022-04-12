# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("//tools/workspace/googletest:repository.bzl", "googletest_repository")

def add_default_repositories():
    """
    Declare workspace repositories for all dependencies. This function should
    be loaded and called from a WORKSPACE file.
    """
    googletest_repository()
