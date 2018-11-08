package(default_visibility = ["//visibility:public"])

cc_library(
    name = "MultiJitteredSampler2D",
    srcs = ["MultiJitteredSampler2D.cc"],
    hdrs = ["MultiJitteredSampler2D.h"],
    deps = [
        ":Sampler2D",
    ]
)

cc_library(
    name = "HemisphereSampler3D",
    srcs = ["HemisphereSampler3D.cc"],
    hdrs = ["HemisphereSampler3D.h"],
    deps = [
        ":Sampler2D",
        "//src/utils:Vec3"
    ]
)

cc_library(
    name = "Sampler2D",
    srcs = ["Sampler2D.cc"],
    hdrs = ["Sampler2D.h"],
)