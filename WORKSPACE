# -*- python -*-
#
# Copyright 2022 Stéphane Caron

workspace(name = "vulp")

BAZEL_VERSION = "4.1.0"
BAZEL_VERSION_SHA_HOST = "0eb2e378d2782e7810753e2162245ad1179c1bb12f848c692b4a595b4edf779b"
BAZEL_VERSION_SHA_PI = "02fcc51686a2f7b360a629747134d62dec885012454fac4c8634fc525884201f"

load("//tools/workspace:default.bzl", "add_default_repositories")
add_default_repositories()

load("@pi3hat//tools/workspace:default.bzl", add_pi3hat_repositories = "add_default_repositories")
add_pi3hat_repositories()

load("@mypy_integration//repositories:repositories.bzl", mypy_integration_repositories = "repositories")
mypy_integration_repositories()

load("@mypy_integration//:config.bzl", "mypy_configuration")
mypy_configuration("//tools/mypy:mypy.ini")

load("@mypy_integration//repositories:deps.bzl", mypy_integration_deps = "deps")
mypy_integration_deps(mypy_requirements_file = "//tools/mypy:version.txt")
