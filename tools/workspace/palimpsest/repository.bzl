# -*- python -*-

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def palimpsest_repository():
    """
    Clone repository from GitHub and make its targets available for binding.
    """
    http_archive(
        name = "palimpsest",
        sha256 = "244ffe888888bc12d6d5270020993a79e56ddb38f2beafa7647f17cf0192d4c9",
        strip_prefix = "palimpsest-2.0.0",
        url = "https://github.com/upkie/palimpsest/archive/refs/tags/v2.0.0.tar.gz",
    )
