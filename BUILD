# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("//tools/lint:lint.bzl", "add_lint_tests")

package(default_visibility = ["//visibility:public"])

exports_files([
    "CPPLINT.cfg",
    ".clang-format",
])

config_setting(
    name = "linux",
    constraint_values = ["@platforms//os:linux"],
)

config_setting(
    name = "osx",
    constraint_values = ["@platforms//os:osx"],
)

config_setting(
    name = "pi32_config",
    values = {
        "cpu": "armeabihf",
    }
)

config_setting(
    name = "pi64_config",
    values = {
        "cpu": "aarch64",
    }
)

py_library(
    name = "python",
    imports = ["."],
    legacy_create_init = False,
    deps = [
        "//vulp:python",
    ],
)

add_lint_tests()
