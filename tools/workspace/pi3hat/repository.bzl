# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

def pi3hat_repository():
    """
    Clone repository from GitHub and make its targets available for binding.
    """
    git_repository(
        name = "pi3hat",
        remote = "https://github.com/mjbots/pi3hat",
        commit = "9512614d8998736e3aa29e8f7b507e3e836a5268",
        shallow_since = "1652690499 +0200"
    )
