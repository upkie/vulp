# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("//tools/lint:lint.bzl", "add_lint_tests")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "datetime_now_string",
    hdrs = ["datetime_now_string.h"],
)

cc_library(
    name = "get_log_path",
    hdrs = ["get_log_path.h"],
    deps = [
        ":datetime_now_string",
    ],
    include_prefix = "vulp/utils",
)

cc_library(
    name = "handle_interrupts",
    hdrs = ["handle_interrupts.h"],
    srcs = [
        "handle_interrupts.cpp",
    ],
    include_prefix = "vulp/utils",
)

cc_library(
    name = "low_pass_filter",
    hdrs = ["low_pass_filter.h"],
    deps = [
        "//vulp/exceptions:filter_error",
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
    name = "synchronous_clock",
    hdrs = ["SynchronousClock.h"],
    srcs = [
        "SynchronousClock.cpp",
    ],
    deps = [
        ":math",
        "@spdlog",
    ],
    include_prefix = "vulp/utils",
)

cc_library(
    name = "utils",
    deps = [
        ":datetime_now_string",
        ":get_log_path",
        ":handle_interrupts",
        ":low_pass_filter",
        ":math",
        ":random_string",
        ":realtime",
        ":synchronous_clock",
    ],
    include_prefix = "vulp/utils",
)

py_library(
    name = "serialize",
    srcs = ["serialize.py"],
)

py_library(
    name = "python",
    srcs = ["__init__.py"],
    deps = [
        ":serialize",
    ],
)

add_lint_tests()
