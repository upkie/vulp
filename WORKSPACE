# -*- python -*-
#
# Copyright 2022 Stéphane Caron

workspace(name = "vulp")

load("//tools/workspace:default.bzl", "add_default_repositories")
add_default_repositories()

# @palimpsest is a default repositories
load("@palimpsest//tools/workspace:default.bzl", add_palimpsest_repositories = "add_default_repositories")
add_palimpsest_repositories()

# @pi3hat is a default repository
load("@pi3hat//tools/workspace:default.bzl", add_pi3hat_repositories = "add_default_repositories")
add_pi3hat_repositories()

# @rpi_bazel is a @pi3hat repository
load("@rpi_bazel//tools/workspace:default.bzl", add_rpi_bazel_repositories = "add_default_repositories")
add_rpi_bazel_repositories()
