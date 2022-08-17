# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("//tools/lint:lint.bzl", "add_lint_tests")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "stop_commands",
    hdrs = [
        "stop_commands.h",
    ],
    deps = [
        "//actuation/moteus",
    ],
    include_prefix = "vulp/spine",
)

cc_library(
    name = "position_commands",
    hdrs = [
        "position_commands.h",
    ],
    deps = [
        "@palimpsest",
        "//actuation/moteus",
    ],
    include_prefix = "vulp/spine",
)

cc_library(
    name = "request",
    hdrs = [
        "Request.h",
    ],
    srcs = [
    ],
    deps = [
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
        "//utils:handle_interrupts",
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
    linkopts = [
        "-lrt",
    ],
    deps = [
        "//actuation:imu_data",
        "//actuation:interface",
        "//observation:observe_imu",
        "//observation:observe_servos",
        "//observation:observe_time",
        "//observation:observer_pipeline",
        "//utils:realtime",
        "//utils:synchronous_clock",
        ":position_commands",
        ":state_machine",
        ":stop_commands",
        "@mpacklog:cpp",
    ],
    include_prefix = "vulp/spine",
)

py_library(
    name = "python",
    deps = select({
        # TODO(scaron): cross-compiled pip install for the Raspberry Pi
        # Related issue: https://github.com/bazelbuild/rules_python/issues/260
        "//:pi32_config": [],
        "//:pi64_config": [],
        "//conditions:default": [
            "@pip_vulp//pypi__msgpack",
            "@pip_vulp//pypi__posix_ipc",
        ],
    }) + [
        "//utils:serialize",
    ],
    srcs = [
        "__init__.py",
        "request.py",
        "spine_error.py",
        "spine_interface.py",
    ],
)

add_lint_tests(enable_clang_format_lint = True)
