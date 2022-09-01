# -*- python -*-

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

def mpacklog_repository():
    """
    Clone repository from GitHub and make its targets available for binding.
    """
    git_repository(
        name = "mpacklog",
        remote = "https://github.com/stephane-caron/mpacklog.git",
        commit = "72abad5a574b32e31c8f79cfd99a9aa40d53c4f3",
        shallow_since = "1662020336 +0200",
    )
