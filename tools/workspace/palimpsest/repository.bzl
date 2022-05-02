# -*- python -*-

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

def palimpsest_repository():
    """
    Clone repository from GitHub and make its targets available for binding.
    """
    git_repository(
        name = "palimpsest",
        remote = "https://github.com/stephane-caron/palimpsest.git",
        commit = "eda89d4f513b4a154534644bdc53a39c0137fe13",
        shallow_since = "1650629043 +0200"
    )
