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
)

py_library(
    name = "python",
    deps = select({
        # TODO(scaron): cross-compiled pip install for the Raspberry Pi
        # Related issue: https://github.com/bazelbuild/rules_python/issues/260
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
