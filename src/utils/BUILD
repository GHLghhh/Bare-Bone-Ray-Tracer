package(default_visibility = ["//visibility:public"])

cc_library(
    name = "constants",
    hdrs = ["constants.h"],
)

cc_library(
    name = "LatticeNoise",
    srcs = ["LatticeNoise.cc"],
    hdrs = ["LatticeNoise.h"],
    deps = [
        ":Vec3",
    ]
)

cc_library(
    name = "CubicNoise",
    srcs = ["CubicNoise.cc"],
    hdrs = ["CubicNoise.h"],
    deps = [
        ":LatticeNoise",
        ":Vec3",
    ]
)

cc_library(
    name = "predefinedScenes",
    srcs = ["predefinedScenes.h"],
    deps = [
        "//src:Ray",
        "//src:World",
        "//src/thirdParty:png",
        "//src/thirdParty:rgbapixel",
        "//src/geometricObjects:Plane",
        "//src/geometricObjects:Sphere",
        "//src/geometricObjects:Triangle",
        "//src/materials:Material",
        "//src/materials:Checker3D",
        "//src/materials:fBmNoiseTexture",
        "//src/samplers:MultiJitteredSampler2D",
        "//src/samplers:Sampler2D",
        "//src/utils/io:loadMeshes",
        ":RGBColor",
        ":Vec3",
    ],
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
        "//src/materials:Material",
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