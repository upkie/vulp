# -*- python -*-

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

def mpacklog_repository():
    """
    Clone repository from GitHub and make its targets available for binding.
    """
    git_repository(
        name = "mpacklog",
        remote = "https://github.com/tasts-robots/mpacklog.git",
        commit = "cf60c19ceed1a31bf88a03645e313f1bd3ec4622",
        shallow_since = "1684244445 +0200",
    )
