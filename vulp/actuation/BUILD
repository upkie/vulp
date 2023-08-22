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
)

cc_library(
    name = "servo_layout",
    hdrs = [
        "ServoLayout.h",
    ],
)

cc_library(
    name = "resolution",
    hdrs = [
        "resolution.h",
    ],
    deps = [
        "//vulp/actuation/moteus",
    ],
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
        "//vulp/actuation/moteus",
        ":imu_data",
        ":resolution",
        ":servo_layout",
        "@palimpsest",
    ],
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
        "//vulp/actuation:interface",
        "//vulp/utils:synchronous_clock",
        "@eigen",
    ],
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
        "//vulp/actuation/bullet/plane",
    ],
    deps = [
        "//vulp/actuation:interface",
        "//vulp/utils:synchronous_clock",
        "@bazel_tools//tools/cpp/runfiles",
        "@bullet",
        "@eigen",
        "@palimpsest",
    ],
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
        "//vulp/utils:realtime",
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