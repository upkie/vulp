# -*- python -*-

load("//tools/workspace:pkg_config.bzl", "pkg_config_repository")

def bullet_pc_repository(**kwargs):
    """
    Make the Bullet target available for binding.
    """
    pkg_config_repository(
        name = "bullet_pc",
        modname = "bullet",
        atleast_version = "3.19",
    )
