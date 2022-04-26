# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("//tools/workspace:github_archive.bzl", "github_archive")

def rules_foreign_cc_repository(version = "0.3.0"):
    """
    Download repository from GitHub as a ZIP archive, decompress it, and make
    its targets available for binding.

    Args:
        version: version of the library to download.
    """
    github_archive(
        name = "rules_foreign_cc",
        repository = "bazelbuild/rules_foreign_cc",
        commit = version,
        strip_prefix = "rules_foreign_cc-{}".format(version),
        sha256 = "a6454385de407afa1ab4c2e5144fca82bea4c40489b79fdcea532fc089051d02",
        build_file = "//tools/workspace/rules_foreign_cc:package.BUILD",
    )
