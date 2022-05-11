# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("//tools/lint:lint.bzl", "add_lint_tests")

package(default_visibility = ["//visibility:public"])

cc_test(
    name = "tests",
    srcs = glob([
        "*.cpp",
        "*.h",
    ]),
    deps = [
        "//legacy/command_line",
        "//vulp/spine:spine",
        "@googletest//:main",
        "@palimpsest",
        "@vulp//actuation:mock_interface",
        "@vulp//utils:random_string",
    ],
)

add_lint_tests(enable_clang_format_lint = True)
