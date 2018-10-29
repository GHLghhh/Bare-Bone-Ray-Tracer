package(default_visibility = ["//visibility:public"])

cc_library(
    name = "constants",
    hdrs = ["constants.h"],
)

cc_library(
    name = "RGBColor",
    srcs = ["RGBColor.cc"],
    hdrs = ["RGBColor.h"],
    deps = [
        ":Vec3",
    ]
)

cc_library(
    name = "ShadeRec",
    srcs = ["ShadeRec.cc"],
    hdrs = ["ShadeRec.h"],
    deps = [
        ":RGBColor",
        ":Vec3",
    ]
)

cc_library(
    name = "utils",
    srcs = ["utils.cc"],
    hdrs = ["utils.h"],
)

cc_library(
    name = "Vec3",
    srcs = ["Vec3.cc"],
    hdrs = ["Vec3.h"],
)