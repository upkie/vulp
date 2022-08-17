# -*- python -*-

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

def mpacklog_repository():
    """
    Clone repository from GitHub and make its targets available for binding.
    """
    git_repository(
        name = "mpacklog",
        remote = "https://github.com/stephane-caron/mpacklog.git",
        commit = "7558b7f88cea91615780684d269ded8b54aeed56",
        shallow_since = "1660727950 +0200"
    )
