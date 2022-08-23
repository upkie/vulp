# -*- python -*-

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

def mpacklog_repository():
    """
    Clone repository from GitHub and make its targets available for binding.
    """
    git_repository(
        name = "mpacklog",
        remote = "https://github.com/stephane-caron/mpacklog.git",
        commit = "19e6e6de1e107e5b5b03a5b30a7326024a992212",
        shallow_since = "1660728440 +0200"
    )
