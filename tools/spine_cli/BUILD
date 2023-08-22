# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("//tools/lint:lint.bzl", "add_lint_tests")

package(default_visibility = ["//visibility:public"])

py_binary(
    name = "spine_cli",
    srcs = [
        "spine_cli.py",
    ],
    deps = [
        "//vulp/spine:python",
    ],
)

add_lint_tests()
