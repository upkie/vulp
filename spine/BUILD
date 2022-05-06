# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("//tools/lint:lint.bzl", "add_lint_tests")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "stop_commands",
    hdrs = [
        "stop_commands.h",
    ],
    deps = [
        "@vulp//actuation/moteus",
    ],
)

add_lint_tests(enable_clang_format_lint = True)
