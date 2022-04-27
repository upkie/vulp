# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("//tools/lint:lint.bzl", "add_lint_tests")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "datetime_now_string",
    hdrs = ["datetime_now_string.h"],
    include_prefix = "vulp/utils",
)

cc_library(
    name = "random_string",
    hdrs = ["random_string.h"],
    include_prefix = "vulp/utils",
)

cc_library(
    name = "utils",
    deps = [
        ":datetime_now_string",
        ":random_string",
    ],
)

add_lint_tests(enable_clang_format_lint = True)
