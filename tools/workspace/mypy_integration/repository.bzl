# -*- python -*-

load("//tools/workspace:github_archive.bzl", "github_archive")

def mypy_integration_repository(
        version = "0.1.0",
        sha256 = "dc6260375d0996f9f9ad7331721837d093a3edab9d8618de33230817d80ea9c1"):
    """
    Download repository from GitHub as a ZIP archive, decompress it, and make
    its targets available for binding.

    Args:
        version: version of the library to download.
        sha256: SHA-256 checksum of the downloaded archive.
    """
    github_archive(
        name = "mypy_integration",
        repository = "thundergolfer/bazel-mypy-integration",
        commit = version,
        strip_prefix = "bazel-mypy-integration-{}".format(version),
        sha256 = sha256,
    )
