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
    name = "pi_config",
    values = {
        "cpu": "armeabihf",
    }
)

add_lint_tests()
