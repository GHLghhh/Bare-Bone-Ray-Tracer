package(default_visibility = ["//visibility:public"])

cc_library(
    name = "Shader",
    srcs = ["Shader.cc"],
    hdrs = ["Shader.h"],
    deps = [
        "//src:Light",
        "//src/materials:Material",
        "//src/utils:RGBColor",
        "//src/utils:ShadeRec",
    ]
)

cc_library(
    name = "BRDF",
    srcs = ["BRDF.cc"],
    hdrs = ["BRDF.h"],
    deps = [
        "//src/utils:RGBColor",
        "//src/utils:ShadeRec",
        "//src/utils:Vec3",
    ]
)

cc_library(
    name = "LambertianBRDF",
    srcs = ["LambertianBRDF.cc"],
    hdrs = ["LambertianBRDF.h"],
    deps = [
        ":BRDF",
        "//src/samplers:HemisphereSampler3D",
        "//src/utils:constants",
    ]
)

cc_library(
    name = "PerfectSpecularBRDF",
    srcs = ["PerfectSpecularBRDF.cc"],
    hdrs = ["PerfectSpecularBRDF.h"],
    deps = [
        ":BRDF",
        "//src/utils:constants",
    ]
)

cc_library(
    name = "GlossySpecularBRDF",
    srcs = ["GlossySpecularBRDF.cc"],
    hdrs = ["GlossySpecularBRDF.h"],
    deps = [
        ":BRDF",
        "//src/samplers:HemisphereSampler3D",
        "//src/utils:constants",
    ]
)
