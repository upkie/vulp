# -*- python -*-

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

def palimpsest_repository():
    """
    Clone repository from GitHub and make its targets available for binding.
    """
    git_repository(
        name = "palimpsest",
        remote = "https://github.com/tasts-robots/palimpsest.git",
        commit = "58a4007b6a1eb5bf3ef8829b364cad9cc127dee5",
        shallow_since = "1696869735 +0200"
    )
