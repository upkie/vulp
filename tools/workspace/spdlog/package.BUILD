# -*- python -*-
#
# Copyright 2022 Stéphane Caron
#
# We build spdlog from source for faster compilation.

package(
    default_visibility = ["//visibility:public"],
)

cc_library(
    name = "spdlog",
    hdrs = glob([
        "include/spdlog/**",
    ]),
    defines = [
        "SPDLOG_FMT_EXTERNAL",
    ],
    includes = ["include"],
    deps = [
        "@fmt",
    ],
)
