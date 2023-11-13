# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("//tools/lint:lint.bzl", "add_lint_tests")
load("@pip_vulp//:requirements.bzl", "requirement")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "agent_interface",
    hdrs = [
        "AgentInterface.h",
        "Request.h",
    ],
    srcs = [
        "AgentInterface.cpp",
    ],
    deps = [
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
    srcs = glob([
        "*.py",
    ]),
    deps = select({
        # Selecting the right CPU architecture for the Raspberry Pi is
        # currently a tough nut to crack for Bazel. We only enable compiled
        # dependencies on the host side for now.
        # Followed in: https://github.com/upkie/upkie/issues/1
        "//:pi64_config": [],
        "//conditions:default": [
            requirement("msgpack"),
            requirement("posix_ipc"),
        ],
    }),
)

add_lint_tests()
