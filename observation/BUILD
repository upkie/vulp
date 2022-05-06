# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("//tools/lint:lint.bzl", "add_lint_tests")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "observer_error",
    hdrs = ["ObserverError.h"],
    include_prefix = "vulp/observation",
)

cc_library(
    name = "observe_servos",
    hdrs = ["observe_servos.h"],
    srcs = ["observe_servos.cpp"],
    deps = [
        "@palimpsest",
        "@vulp//actuation/moteus",
    ],
    include_prefix = "vulp/observation",
)

cc_library(
    name = "observe_time",
    hdrs = ["observe_time.h"],
    srcs = ["observe_time.cpp"],
    deps = [
        "@palimpsest",
    ],
    include_prefix = "vulp/observation",
)

cc_library(
    name = "observer",
    hdrs = ["Observer.h"],
    deps = [
        "@palimpsest",
    ],
    include_prefix = "vulp/observation",
)

cc_library(
    name = "source",
    hdrs = ["Source.h"],
    deps = [
        "@palimpsest",
    ],
    include_prefix = "vulp/observation",
)

cc_library(
    name = "observer_pipeline",
    hdrs = ["ObserverPipeline.h"],
    srcs = ["ObserverPipeline.cpp"],
    deps = [
        ":observer_error",
        ":observer",
        ":source",
    ],
    include_prefix = "vulp/observation",
)

cc_library(
    name = "observation",
    deps = [
        ":observer",
        ":observer_error",
        ":source",
        ":observe_servos",
        ":observe_time",
        ":observer_pipeline",
    ],
)

add_lint_tests(enable_clang_format_lint = True)
