# -*- python -*-

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

def palimpsest_repository():
    """
    Clone repository from GitHub and make its targets available for binding.
    """
    git_repository(
        name = "palimpsest",
        remote = "https://github.com/upkie/palimpsest",
        commit = "a8459cb95b4cfe46a5c521eaa976c690b13a1542",
        shallow_since = "1712924252 +0200"
    )
