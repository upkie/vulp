# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("//tools/lint:lint.bzl", "add_lint_tests")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "random_string",
    hdrs = ["random_string.h"],
)

cc_library(
    name = "utils",
    deps = [
        ":random_string",
    ],
)

add_lint_tests(enable_clang_format_lint = True)
