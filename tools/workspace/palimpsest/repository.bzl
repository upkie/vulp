# -*- python -*-

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

def palimpsest_repository():
    """
    Clone repository from GitHub and make its targets available for binding.
    """
    git_repository(
        name = "palimpsest",
        remote = "https://github.com/stephane-caron/palimpsest.git",
        commit = "0314398f153faea13a387db5425a5ed2d2b54be1",
        shallow_since = "1653669769 +0200"
    )
