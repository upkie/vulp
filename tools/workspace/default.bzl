# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("//tools/workspace/bullet:repository.bzl", "bullet_repository")
load("//tools/workspace/eigen:repository.bzl", "eigen_repository")
load("//tools/workspace/fmt:repository.bzl", "fmt_repository")
load("//tools/workspace/googletest:repository.bzl", "googletest_repository")
load("//tools/workspace/mpack:repository.bzl", "mpack_repository")
load("//tools/workspace/mypy_integration:repository.bzl", "mypy_integration_repository")
load("//tools/workspace/palimpsest:repository.bzl", "palimpsest_repository")
load("//tools/workspace/pi3hat:repository.bzl", "pi3hat_repository")
load("//tools/workspace/posix_ipc:repository.bzl", "posix_ipc_repository")
load("//tools/workspace/pycodestyle:repository.bzl", "pycodestyle_repository")
load("//tools/workspace/rules_python:repository.bzl", "rules_python_repository")
load("//tools/workspace/spdlog:repository.bzl", "spdlog_repository")
load("//tools/workspace/styleguide:repository.bzl", "styleguide_repository")
load("//tools/workspace/upkie_description:repository.bzl", "upkie_description_repository")

def add_default_repositories():
    """
    Declare workspace repositories for all dependencies. This function should
    be loaded and called from a WORKSPACE file.
    """
    bullet_repository()
    eigen_repository()
    fmt_repository()
    googletest_repository()
    mpack_repository()
    mypy_integration_repository()
    palimpsest_repository()
    pi3hat_repository()
    posix_ipc_repository()
    pycodestyle_repository()
    rules_python_repository()
    spdlog_repository()
    styleguide_repository()
    upkie_description_repository()
