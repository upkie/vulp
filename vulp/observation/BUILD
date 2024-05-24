# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("//tools/lint:lint.bzl", "add_lint_tests")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "observe_servos",
    hdrs = ["observe_servos.h"],
    srcs = ["observe_servos.cpp"],
    deps = [
        "//vulp/actuation/moteus",
        "@palimpsest",
    ],
    include_prefix = "vulp/observation",
)

cc_library(
    name = "observe_time",
    hdrs = ["observe_time.h"],
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
    name = "history_observer",
    hdrs = ["HistoryObserver.h"],
    deps = [
        "@vulp//vulp/exceptions",
        ":observer",
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
    srcs = [
        "ObserverPipeline.cpp",
    ],
    deps = [
        "//vulp/exceptions:observer_error",
        ":observer",
        ":source",
    ],
    include_prefix = "vulp/observation",
)

cc_library(
    name = "observation",
    deps = [
        ":observe_servos",
        ":observe_time",
        ":observer",
        ":observer_pipeline",
        ":source",
    ],
    include_prefix = "vulp/observation",
)

add_lint_tests()
