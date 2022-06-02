# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("//tools/lint:lint.bzl", "add_lint_tests")

package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "clean",
    srcs = ["clean.cpp"],
    linkopts = [
        "-lrt",
    ],
    deps = [
        "@spdlog",
    ],
)

add_lint_tests(enable_clang_format_lint = True)
