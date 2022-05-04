# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("//tools/lint:lint.bzl", "add_lint_tests")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "observer",
    hdrs = ["Observer.h"],
    deps = [
        "@palimpsest",
    ],
    include_prefix = "vulp/observation",
)

cc_library(
    name = "source",
    hdrs = ["Source.h"],
    deps = [
        "@palimpsest",
    ],
    include_prefix = "vulp/observation",
)

cc_library(
    name = "observation",
    deps = [
        ":observer",
        ":source",
    ],
)

add_lint_tests(enable_clang_format_lint = True)
