# -*- python -*-

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def mpack_repository():
    """
    Download repository from GitHub as a ZIP archive, decompress it, and make
    its targets available for binding.
    """
    http_archive(
        name = "mpack",
        url = "https://github.com/ludocode/mpack/releases/download/v1.0/mpack-amalgamation-1.0.tar.gz",
        strip_prefix = "mpack-amalgamation-1.0/src/mpack",
        sha256 = "25c2f22a30b5207288ffededc092c7d2cb5599cc0eea9d5787d738088429cd56",
        build_file = "//tools/workspace/{name}:package.BUILD".format(name="mpack")
    )
