# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("//tools/lint:lint.bzl", "add_lint_tests")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "imu_data",
    hdrs = ["ImuData.h"],
    include_prefix = "vulp/actuation",
)

cc_library(
    name = "resolution",
    hdrs = ["resolution.h"],
    deps = [
        "//actuation/moteus",
    ],
    include_prefix = "vulp/actuation",
)

add_lint_tests(enable_clang_format_lint = True)
