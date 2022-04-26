# -*- python -*-

load("//tools/workspace:github_archive.bzl", "github_archive")

def spdlog_repository(
        version = "1.8.5",
        sha256 = "6e66c8ed4c014b0fb00c74d34eea95b5d34f6e4b51b746b1ea863dc3c2e854fd"):
    """
    Download repository from GitHub as a ZIP archive, decompress it, and make
    its targets available for binding.

    Args:
        version: version of the library to get.
        sha256: SHA-256 checksum of the downloaded archive.
    """
    github_archive(
        name = "spdlog",
        repository = "gabime/spdlog",
        commit = "v{}".format(version),
        strip_prefix = "spdlog-{}".format(version),
        sha256 = sha256,
        build_file = "//tools/workspace/{repo}:package.BUILD".format(repo="spdlog"),
    )
