# -*- python -*-

load("//tools/lint:lint.bzl", "add_lint_tests")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "filter_error",
    hdrs = ["FilterError.h"],
    include_prefix = "vulp/exceptions",
)

cc_library(
    name = "observer_error",
    hdrs = ["ObserverError.h"],
    include_prefix = "vulp/exceptions",
)

cc_library(
    name = "exceptions",
    deps = [
        ":filter_error",
        ":observer_error",
    ],
    include_prefix = "vulp/exceptions",
)

add_lint_tests()
