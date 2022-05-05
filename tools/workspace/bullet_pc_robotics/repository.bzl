# -*- python -*-

load("//tools/workspace:pkg_config.bzl", "pkg_config_repository")

def bullet_pc_robotics_repository():
    """
    Make the BulletRobotics target available for binding.
    """
    pkg_config_repository(
        name = "bullet_pc_robotics",
        modname = "bullet_robotics",
        atleast_version = "3.19",
        extra_defines = [],
    )
