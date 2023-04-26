# -*- python -*-

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

def mpacklog_repository():
    """
    Clone repository from GitHub and make its targets available for binding.
    """
    git_repository(
        name = "mpacklog",
        remote = "https://github.com/tasts-robots/mpacklog.git",
        commit = "093ca15e7eb90275da0554bfe283cb36c390e95d",
        shallow_since = "1682497613 +0200",
    )
