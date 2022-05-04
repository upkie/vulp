# -*- python -*-

load("//tools/workspace:pkg_config.bzl", "pkg_config_repository")

def bullet_robotics_gui_repository():
    """
    Make the BulletRoboticsGUI target available for binding.
    """
    pkg_config_repository(
        name = "bullet_robotics_gui",
        modname = "bullet_robotics_gui",
        atleast_version = "3.19",
        extra_defines = [],
    )
