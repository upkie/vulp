# -*- python -*-

load("//tools/workspace:pkg_config.bzl", "pkg_config_repository")

def bullet_robotics_repository():
    """
    Make the BulletRobotics target available for binding.
    """
    pkg_config_repository(
        name = "bullet_robotics",
        modname = "bullet_robotics",
        atleast_version = "3.19",
        extra_defines = [],
    )
