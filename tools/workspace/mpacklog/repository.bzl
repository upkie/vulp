# -*- python -*-

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

def mpacklog_repository():
    """
    Clone repository from GitHub and make its targets available for binding.
    """
    git_repository(
        name = "mpacklog",
        remote = "https://github.com/stephane-caron/mpacklog.git",
        commit = "6b8be72e7f2a3bbd620e9d478d53be294a33d214",
        shallow_since = "1660727671 +0200"
    )
