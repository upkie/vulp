# -*- python -*-
#
# Copyright 2022 Stéphane Caron

workspace(name = "vulp")

load("//tools/workspace:default.bzl", "add_default_repositories")
add_default_repositories()

# @palimpsest is a default repositories
load("@palimpsest//tools/workspace:default.bzl", add_palimpsest_repositories = "add_default_repositories")
add_palimpsest_repositories()

# We can load this now that @rules_python has been added as a @palimpsest repository
load("//tools/workspace/pip_vulp:parse_deps.bzl", "parse_deps")
parse_deps()
load("@pip_vulp//:requirements.bzl", "install_deps")
install_deps()

# @pi3hat is a default repository
load("@pi3hat//tools/workspace:default.bzl", add_pi3hat_repositories = "add_default_repositories")
add_pi3hat_repositories()

# @rpi_bazel is a @pi3hat repository
load("@rpi_bazel//tools/workspace:default.bzl", add_rpi_bazel_repositories = "add_default_repositories")
add_rpi_bazel_repositories()

load("@mypy_integration//repositories:repositories.bzl", mypy_integration_repositories = "repositories")
mypy_integration_repositories()

load("@mypy_integration//:config.bzl", "mypy_configuration")
mypy_configuration("//tools/mypy:mypy.ini")

load("@mypy_integration//repositories:deps.bzl", mypy_integration_deps = "deps")
mypy_integration_deps(mypy_requirements_file = "//tools/mypy:version.txt")
