# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("//tools/lint:lint.bzl", "add_lint_tests")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "circular_buffer",
    hdrs = ["CircularBuffer.h"],
    deps = [
        "@spdlog",
    ],
    include_prefix = "vulp/utils",
)

cc_library(
    name = "datetime_now_string",
    hdrs = ["datetime_now_string.h"],
    include_prefix = "vulp/utils",
)

cc_library(
    name = "handle_interrupts",
    hdrs = ["handle_interrupts.h"],
    srcs = ["handle_interrupts.cpp"],
    include_prefix = "vulp/utils",
)

cc_library(
    name = "low_pass_filter",
    hdrs = [
        "low_pass_filter.h",
        "LowPassFilter.h",
    ],
    deps = [
        "@eigen",
    ],
    include_prefix = "vulp/utils",
)

cc_library(
    name = "math",
    hdrs = ["math.h"],
    include_prefix = "vulp/utils",
)

cc_library(
    name = "random_string",
    hdrs = ["random_string.h"],
    include_prefix = "vulp/utils",
)

cc_library(
    name = "realtime",
    hdrs = ["realtime.h"],
    include_prefix = "vulp/utils",
)

cc_library(
    name = "spinlock",
    hdrs = ["Spinlock.h"],
    include_prefix = "vulp/utils",
)

cc_library(
    name = "synchronous_clock",
    hdrs = ["SynchronousClock.h"],
    srcs = ["SynchronousClock.cpp"],
    deps = [
        ":math",
        "@spdlog",
    ],
    include_prefix = "vulp/utils",
)

cc_library(
    name = "utils",
    deps = [
        ":circular_buffer",
        ":datetime_now_string",
        ":handle_interrupts",
        ":low_pass_filter",
        ":math",
        ":random_string",
        ":realtime",
        ":spinlock",
        ":synchronous_clock",
    ],
)

add_lint_tests(enable_clang_format_lint = True)
