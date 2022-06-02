# -*- python -*-

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

def palimpsest_repository():
    """
    Clone repository from GitHub and make its targets available for binding.
    """
    git_repository(
        name = "palimpsest",
        remote = "https://github.com/stephane-caron/palimpsest.git",
        commit = "4a2d4935ca3394cb036cfad4c9030d3b22b8cc42",
        shallow_since = "1654192305 +0200"
    )
