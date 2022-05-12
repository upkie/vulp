# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("//tools/lint:lint.bzl", "add_lint_tests")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "imu_data",
    hdrs = [
        "ImuData.h",
    ],
    deps = [
        "@eigen",
    ],
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
    name = "bullet_interface",
    hdrs = [
        "BulletImuData.h",
        "BulletInterface.h",
        "bullet_utils.h",
    ],
    srcs = [
        "BulletInterface.cpp",
    ],
    data = [
        "//actuation/bullet/plane",
    ],
    deps = [
        "@bazel_tools//tools/cpp/runfiles",
        "@bullet",
        "@eigen",
        "@palimpsest",
        "@vulp//actuation:interface",
        "@vulp//utils:synchronous_clock",
    ],
    include_prefix = "vulp/actuation",
)

cc_library(
    name = "pi3hat_interface",
    hdrs = [
        "Pi3HatInterface.h",
    ],
    srcs = [
        "Pi3HatInterface.cpp",
    ],
    deps = [
        "//utils:realtime",
        ":interface",
        "@pi3hat//lib/cpp/mjbots/pi3hat:libpi3hat",
    ] + select({
        "//:pi_config": ["@org_llvm_libcxx//:libcxx"],
        "//conditions:default": [],
    }),
    include_prefix = "vulp/actuation",
)

cc_library(
    name = "actuation",
    deps = [
        ":bullet_interface",
        ":mock_interface",
        ":pi3hat_interface",
    ],
)

add_lint_tests(enable_clang_format_lint = True)
