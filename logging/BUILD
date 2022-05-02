# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("//tools/lint:lint.bzl", "add_lint_tests")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "logger",
    hdrs = ["Logger.h"],
    srcs = ["Logger.cpp"],
    copts = [
        "-pthread",
    ],
    linkopts = [
        "-pthread",
    ],
    deps = [
        "@palimpsest",
        "//utils:circular_buffer",
    ],
)

cc_library(
    name = "logging",
    deps = [
        ":logger",
    ],
)

add_lint_tests(enable_clang_format_lint = True)
