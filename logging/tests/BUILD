# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("//tools/lint:lint.bzl", "add_lint_tests")

package(default_visibility = ["//visibility:public"])

cc_test(
    name = "tests",
    srcs = glob(["*.cpp"]),
    deps = [
        "//logging",
        "//utils:random_string",
        "//utils:synchronous_clock",
        "@eigen",
        "@googletest//:main",
    ],
)

add_lint_tests(enable_clang_format_lint = True)
