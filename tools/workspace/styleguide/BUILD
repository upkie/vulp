# -*- python -*-
#
# Copyright 2022 Stéphane Caron

load("//tools/lint:lint.bzl", "add_lint_tests")

py_test(
    name = "cpplint_unittest",
    size = "small",
    srcs = ["@styleguide//:cpplint_unittest"],
    tags = ["no_kcov"],
)

add_lint_tests()
