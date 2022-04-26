# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("//tools/workspace:github_archive.bzl", "github_archive")

def fmt_repository(
        version = "7.1.3",
        sha256 = "50f2fd9f697f89726ae3c7efe84ae48c9e03158a2958eea496eeaa0fb190adb6"):
    """
    Download repository from GitHub as a ZIP archive, decompress it, and make
    its targets available for binding.

    Args:
        version: version of the library to get.
        sha256: SHA-256 checksum of the downloaded archive.
    """
    github_archive(
        name = "fmt",
        repository = "fmtlib/fmt",
        commit = version,
        sha256 = sha256,
        build_file = "//tools/workspace/{repo}:package.BUILD".format(repo="fmt"),
    )
