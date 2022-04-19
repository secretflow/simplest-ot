load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "simplest_ot_portable",
    srcs = glob(
        [
            "simplest_ot_portable/*.c",
        ],
        exclude = ["simplest_ot_portable/*_test.c"],
    ),
    hdrs = glob(["simplest_ot_portable/*.h"]),
    linkstatic = True,
    visibility = ["//visibility:public"],
)

cc_library(
    name = "simplest_ot_x86_asm",
    srcs = glob(
        [
            "simplest_ot_x86_asm/*.c",
            "simplest_ot_x86_asm/*.S",
        ],
        exclude = ["simplest_ot_x86_asm/*_test.c"],
    ),
    hdrs = glob(["simplest_ot_x86_asm/*.h"]),
    linkstatic = True,
    textual_hdrs = glob(["simplest_ot_x86_asm/*.data"]),
    visibility = ["//visibility:public"],
)
