# -*- python -*-

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

def mpacklog_repository():
    """
    Clone repository from GitHub and make its targets available for binding.
    """
    git_repository(
        name = "mpacklog",
        remote = "https://github.com/tasts-robots/mpacklog.git",
        commit = "899db5309a99dae8bc540e7da1849b760d7f45f5",
        shallow_since = "1691501536 +0200",
    )
