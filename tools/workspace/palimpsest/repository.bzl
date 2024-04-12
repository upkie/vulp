# -*- python -*-

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

def palimpsest_repository():
    """
    Clone repository from GitHub and make its targets available for binding.
    """
    git_repository(
        name = "palimpsest",
        remote = "https://github.com/upkie/palimpsest",
        commit = "c0ea282fbc69e8bfa510fe60f6ae1e582350cfba",
        shallow_since = "1655469414 -0400"
    )
