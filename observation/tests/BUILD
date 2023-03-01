# -*- python -*-
#
# Copyright 2022 Stéphane Caron
# Copyright 2023 Inria

load("//tools/lint:lint.bzl", "add_lint_tests")

package(default_visibility = ["//visibility:public"])

cc_test(
    name = "tests",
    srcs = glob([
        "*.cpp",
        "*.h",
    ]),
    deps = [
        "//observation",
        "@eigen",
        "@googletest//:main",
        "@vulp//actuation/moteus",
    ],
)

cc_library(
    name = "observers",
    hdrs = [
        "SchwiftyObserver.h",
        "ThrowingObserver.h",
    ],
    include_prefix = "vulp/observers/tests",
)

add_lint_tests(enable_clang_format_lint = True)
