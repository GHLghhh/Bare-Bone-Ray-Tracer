package(default_visibility = ["//visibility:public"])

cc_library(
    name = "png",
    srcs = ["png.cpp"],
    hdrs = ["png.h"],
    deps = [
        ":rgbapixel",
        "@png_archive//:png"
    ]
)

cc_library(
    name = "rgbapixel",
    srcs = ["rgbapixel.cpp"],
    hdrs = ["rgbapixel.h"],
)