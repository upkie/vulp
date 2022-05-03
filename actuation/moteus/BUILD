# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("//tools/lint:lint.bzl", "add_lint_tests")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "moteus",
    hdrs = glob(["*.h"]),
    deps = [],
    include_prefix = "vulp/actuation/moteus",
)

add_lint_tests(enable_clang_format_lint = True)
