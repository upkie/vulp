# -*- python -*-

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

def mpacklog_repository():
    """
    Clone repository from GitHub and make its targets available for binding.
    """
    git_repository(
        name = "mpacklog",
        remote = "https://github.com/stephane-caron/mpacklog.git",
        commit = "13e3c305212629590be8d7676d0cbeb31a7a48b7",
        shallow_since = "1660728440 +0200"
    )
