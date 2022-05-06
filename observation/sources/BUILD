# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("//tools/lint:lint.bzl", "add_lint_tests")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "cpu_temperature",
    hdrs = ["CpuTemperature.h"],
    srcs = ["CpuTemperature.cpp"],
    deps = [
        "@vulp//observation:source",
    ],
)

cc_library(
    name = "observers",
    deps = [
        ":cpu_temperature",
    ],
)

add_lint_tests(enable_clang_format_lint = True)
