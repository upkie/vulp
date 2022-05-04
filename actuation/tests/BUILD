# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("//tools/lint:lint.bzl", "add_lint_tests")

package(default_visibility = ["//visibility:public"])

cc_test(
    name = "interface_test",
    srcs = [
        "InterfaceTest.cpp",
    ],
    deps = [
        "//actuation:interface",
        "@googletest//:main",
    ],
)

cc_test(
    name = "mock_interface_test",
    srcs = ["MockInterfaceTest.cpp"],
    deps = [
        "//actuation:mock_interface",
        "@googletest//:main",
    ],
)

add_lint_tests(enable_clang_format_lint = True)
