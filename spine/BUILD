# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("//tools/lint:lint.bzl", "add_lint_tests")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "stop_commands",
    hdrs = [
        "stop_commands.h",
    ],
    deps = [
        "//actuation/moteus",
    ],
    include_prefix = "vulp/spine",
)

cc_library(
    name = "position_commands",
    hdrs = [
        "position_commands.h",
    ],
    deps = [
        "@palimpsest",
        "//actuation/moteus",
    ],
    include_prefix = "vulp/spine",
)

cc_library(
    name = "state_machine",
    hdrs = [
        "StateMachine.h",
    ],
    srcs = [
        "StateMachine.cpp",
    ],
    deps = [
    ],
    include_prefix = "vulp/spine",
)

add_lint_tests(enable_clang_format_lint = True)
