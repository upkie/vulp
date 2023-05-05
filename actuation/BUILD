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
    hdrs = [
        "ServoLayout.h",
    ],
    include_prefix = "vulp/actuation",
)

cc_library(
    name = "resolution",
    hdrs = [
        "resolution.h",
    ],
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
    srcs = [
        "Interface.cpp",
    ],
    deps = [
        "//actuation/moteus",
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
    srcs = select({
        "//:pi32_config": ["Pi3HatInterface.cpp"],
        "//:pi64_config": ["Pi3HatInterface.cpp"],
        "//conditions:default": [],
    }),
    deps = [
        "//utils:realtime",
        ":interface",
    ] + select({
        "//:pi32_config": [
            "@org_llvm_libcxx//:libcxx",
            "@pi3hat//lib/cpp/mjbots/pi3hat:libpi3hat",
        ],
        "//:pi64_config": [
            "@org_llvm_libcxx//:libcxx",
            "@pi3hat//lib/cpp/mjbots/pi3hat:libpi3hat",
        ],
        "//conditions:default": [],
    }),
    include_prefix = "vulp/actuation",
)

cc_library(
    name = "actuation",
    deps = [
        ":bullet_interface",
        ":mock_interface",
    ] + select({
        "//:pi32_config": [":pi3hat_interface"],
        "//:pi64_config": [":pi3hat_interface"],
        "//conditions:default": [],
    }),
)

add_lint_tests()
