# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("//tools/workspace/bazel-compile-commands-extractor:repository.bzl", "bazel_compile_commands_extractor_repository")
load("//tools/workspace/bullet:repository.bzl", "bullet_repository")
load("//tools/workspace/mpacklog:repository.bzl", "mpacklog_repository")
load("//tools/workspace/palimpsest:repository.bzl", "palimpsest_repository")
load("//tools/workspace/pi3hat:repository.bzl", "pi3hat_repository")
load("//tools/workspace/pycodestyle:repository.bzl", "pycodestyle_repository")
load("//tools/workspace/upkie_description:repository.bzl", "upkie_description_repository")

def add_default_repositories():
    """
    Declare workspace repositories for all dependencies. This function should
    be loaded and called from a WORKSPACE file.
    """
    bazel_compile_commands_extractor_repository()
    bullet_repository()
    mpacklog_repository()
    palimpsest_repository()
    pi3hat_repository()
    pycodestyle_repository()
    upkie_description_repository()
