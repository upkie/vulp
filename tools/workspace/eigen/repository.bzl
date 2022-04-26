# -*- python -*-

load("//tools/workspace:pkg_config.bzl", "pkg_config_repository")

def eigen_repository():
    """
    Make Eigen targets available for binding.
    """
    pkg_config_repository(
        name = "eigen",
        modname = "eigen3",
        atleast_version = "3.3.4",
        extra_defines = ["EIGEN_MPL2_ONLY"],
    )
