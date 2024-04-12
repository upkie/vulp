# -*- python -*-

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

def palimpsest_repository():
    """
    Clone repository from GitHub and make its targets available for binding.
    """
    git_repository(
        name = "palimpsest",
        remote = "https://github.com/upkie/palimpsest",
        commit = "f52bcfeeda2da839107d6121e0c8ef1a0c42522e",
        shallow_since = "1655469414 -0400"
    )
