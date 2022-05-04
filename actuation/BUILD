# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("//tools/lint:lint.bzl", "add_lint_tests")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "imu_data",
    hdrs = ["ImuData.h"],
    include_prefix = "vulp/actuation",
)

cc_library(
    name = "servo_layout",
    hdrs = ["ServoLayout.h"],
    include_prefix = "vulp/actuation",
)

cc_library(
    name = "resolution",
    hdrs = ["resolution.h"],
    deps = [
        "//actuation/moteus",
    ],
    include_prefix = "vulp/actuation",
)

cc_library(
    name = "interface",
    hdrs = [
        "Interface.h",
    ],
    deps = [
        ":imu_data",
        ":resolution",
        ":servo_layout",
        "@palimpsest",
    ],
    include_prefix = "vulp/actuation",
)

cc_library(
    name = "mock_interface",
    hdrs = [
        "MockInterface.h",
    ],
    srcs = [
        "MockInterface.cpp",
    ],
    deps = [
        "//actuation:interface",
        "//utils:synchronous_clock",
        "@eigen",
    ],
    include_prefix = "vulp/actuation",
)

cc_library(
    name = "bullet_utils",
    hdrs = [
        "BulletImuData.h",
        "bullet_utils.h",
    ],
    deps = [
        ":imu_data",
        "@bullet",
        "@eigen",
        "@palimpsest",
    ],
    include_prefix = "vulp/actuation",
)

cc_library(
    name = "actuation",
    deps = [
        ":mock_interface",
    ],
)

add_lint_tests(enable_clang_format_lint = True)
