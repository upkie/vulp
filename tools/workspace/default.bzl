# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("//tools/workspace/eigen:repository.bzl", "eigen_repository")
load("//tools/workspace/fmt:repository.bzl", "fmt_repository")
load("//tools/workspace/googletest:repository.bzl", "googletest_repository")
load("//tools/workspace/mpack:repository.bzl", "mpack_repository")
load("//tools/workspace/mypy_integration:repository.bzl", "mypy_integration_repository")
load("//tools/workspace/palimpsest:repository.bzl", "palimpsest_repository")
load("//tools/workspace/pycodestyle:repository.bzl", "pycodestyle_repository")
load("//tools/workspace/rules_foreign_cc:repository.bzl", "rules_foreign_cc_repository")
load("//tools/workspace/rules_python:repository.bzl", "rules_python_repository")
load("//tools/workspace/spdlog:repository.bzl", "spdlog_repository")
load("//tools/workspace/styleguide:repository.bzl", "styleguide_repository")

def add_default_repositories():
    """
    Declare workspace repositories for all dependencies. This function should
    be loaded and called from a WORKSPACE file.
    """
    eigen_repository()
    fmt_repository()
    googletest_repository()
    mpack_repository()
    mypy_integration_repository()
    palimpsest_repository()
    pycodestyle_repository()
    rules_foreign_cc_repository()
    rules_python_repository()
    spdlog_repository()
    styleguide_repository()
