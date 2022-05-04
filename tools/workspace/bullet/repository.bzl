# -*- python -*-

load("//tools/workspace:pkg_config.bzl", "pkg_config_repository")

def bullet_repository(**kwargs):
    """
    Make the Bullet target available for binding.
    """
    pkg_config_repository(
        name = "bullet",
        modname = "bullet",
        atleast_version = "3.19",
    )
