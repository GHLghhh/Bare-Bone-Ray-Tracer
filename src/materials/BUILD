package(default_visibility = ["//visibility:public"])

cc_library(
    name = "Material",
    srcs = ["Material.cc"],
    hdrs = ["Material.h"],
    deps = [
        "//src/utils:RGBColor",
        "//src/utils:Vec3",
    ]
)

cc_library(
    name = "Checker3D",
    srcs = ["Checker3D.cc"],
    hdrs = ["Checker3D.h"],
    deps = [
        ":Material",
    ]
)

cc_library(
    name = "fBmNoiseTexture",
    srcs = ["fBmNoiseTexture.cc"],
    hdrs = ["fBmNoiseTexture.h"],
    deps = [
        ":Material",
        "//src/utils:CubicNoise",
        "//src/utils:LatticeNoise",
    ]
)