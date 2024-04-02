# -*- python -*-

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def bazel_compile_commands_extractor_repository():
    """
    Clone repository from GitHub and make its targets available for binding.

    Hedron's Compile Commands Extractor for Bazel:
    https://github.com/hedronvision/bazel-compile-commands-extractor

    This is an extra tool we use to generate compile_commands.json files that
    can in turn be used by IDEs to find headers in Bazel dependencies.
    """
    http_archive(
        name = "hedron_compile_commands",
        url = "https://github.com/hedronvision/bazel-compile-commands-extractor/archive/5bcb0bd8a917b2b48fb5dc55818515f4be3b63ff.tar.gz",
        strip_prefix = "bazel-compile-commands-extractor-5bcb0bd8a917b2b48fb5dc55818515f4be3b63ff",
        sha256 = "5911a95b1e4e5a5d3c81ee01b4fc9c5ba63048bb2e9f0a623d9fdecfe8c46185",
    )
