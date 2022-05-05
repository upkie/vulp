# -*- python -*-

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

def palimpsest_repository():
    """
    Clone repository from GitHub and make its targets available for binding.
    """
    git_repository(
        name = "palimpsest",
        remote = "https://github.com/stephane-caron/palimpsest.git",
        commit = "4ea456791079c5e44b00e3759f65450cce417e4b",
        shallow_since = "1651657866 +0200"
    )
