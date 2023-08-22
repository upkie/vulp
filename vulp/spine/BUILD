# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("//tools/lint:lint.bzl", "add_lint_tests")
load("@pip_vulp//:requirements.bzl", "requirement")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "request",
    hdrs = [
        "Request.h",
    ],
    include_prefix = "vulp/spine",
)

cc_library(
    name = "agent_interface",
    hdrs = [
        "AgentInterface.h",
    ],
    srcs = [
        "AgentInterface.cpp",
    ],
    deps = [
        ":request",
        "@spdlog",
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
        ":agent_interface",
        "//vulp/utils:handle_interrupts",
    ],
    include_prefix = "vulp/spine",
)

cc_library(
    name = "spine",
    hdrs = [
        "Spine.h",
    ],
    srcs = [
        "Spine.cpp",
    ],
    linkopts = select({
        "@//:linux": ["-lrt"],
        "@//conditions:default": [],
    }),
    deps = [
        "//vulp/actuation:imu_data",
        "//vulp/actuation:interface",
        "//vulp/observation:observe_imu",
        "//vulp/observation:observe_servos",
        "//vulp/observation:observe_time",
        "//vulp/observation:observer_pipeline",
        "//vulp/utils:realtime",
        "//vulp/utils:synchronous_clock",
        ":state_machine",
        "@mpacklog",
    ],
    include_prefix = "vulp/spine",
)

py_library(
    name = "python",
    deps = select({
        # Selecting the right CPU architecture for the Raspberry Pi is
        # currently a tough nut to crack for Bazel. We only enable compiled
        # dependencies on the host side for now.
        # Followed in: https://github.com/tasts-robots/upkie/issues/1
        "//:pi32_config": [],
        "//:pi64_config": [],
        "//conditions:default": [
            requirement("msgpack"),
            requirement("posix_ipc"),
        ],
    }) + [
        "//vulp/utils:serialize",
    ],
    srcs = [
        "__init__.py",
        "request.py",
        "spine_error.py",
        "spine_interface.py",
    ],
)

add_lint_tests()
