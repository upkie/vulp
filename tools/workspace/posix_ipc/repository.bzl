# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("//tools/workspace:pypi.bzl", "pypi_archive")

def posix_ipc_repository(
        version = "1.0.5",
        sha256 = "6cddb1ce2cf4aae383f2a0079c26c69bee257fe2720f372201ef047f8ceb8b97"):
    """
    Download source package from PyPI and make its targets available for
    binding.

    Args:
        version: version of the library to get.
        sha256: SHA-256 checksum of the downloaded archive.
    """
    pypi_archive(
        name = "posix_ipc",
        package = "posix_ipc",
        version = version,
        sha256 = sha256,
        build_file = Label("//tools/workspace/posix_ipc:package.BUILD"),
    )
