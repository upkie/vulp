# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("//tools/lint:lint.bzl", "add_lint_tests")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "control",
    deps = [
        ":controller",
    ],
    include_prefix = "vulp/control",
)

cc_library(
    name = "controller",
    hdrs = ["Controller.h"],
    deps = [
        "@palimpsest",
    ],
    include_prefix = "vulp/control",
)

add_lint_tests()
