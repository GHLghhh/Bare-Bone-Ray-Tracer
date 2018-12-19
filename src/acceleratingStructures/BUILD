package(default_visibility = ["//visibility:public"])

cc_library(
    name = "KDTree",
    srcs = ["KDTree.cc"],
    hdrs = ["KDTree.h"],
    deps = [
        "//src/utils:Vec3",
    ]
)

cc_library(
    name = "IrradianceCache",
    srcs = ["IrradianceCache.cc"],
    hdrs = ["IrradianceCache.h"],
    deps = [
        ":KDTree",
        "//src/utils:constants",
        "//src/utils:Vec3",
        "//src/utils:RGBColor",
    ]
)