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
        commit = "9d657a86b01cb8e07d14960504229c7d2e4e0761",
        shallow_since = "1618844474 -0400"
    )
