# -*- python -*-
#
# Copyright 2022 Stéphane Caron
#
# This file incorporates work covered by the following copyright and permission
# notice:
#
#     Copyright 2012-2016 Robot Locomotion Group @ CSAIL
#     License: BSD-3-Clause

package(
    default_visibility = ["//visibility:public"],
)

cc_library(
    name = "fmt",
    hdrs = glob([
        "include/fmt/*.h",
    ], exclude = [
        # The POSIX bindings are not header-only.
        "include/fmt/posix.h",
    ]),
    defines = [
        # Like Drake, we use the header-only build of fmt.
        "FMT_HEADER_ONLY=1",
        # Avoid macro pollution.
        "FMT_NO_FMT_STRING_ALIAS=1",
    ],
    includes = ["include"],
)
