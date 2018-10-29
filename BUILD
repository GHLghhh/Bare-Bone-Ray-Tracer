package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "ray_tracer",
    srcs = ["main.cc"],
    deps = [
        ":Ray",
        ":World",
        "//thirdParty:png",
        "//thirdParty:rgbapixel",
        "//geometricObjects:Plane",
        "//geometricObjects:Sphere",
        "//geometricObjects:Triangle",
        "//samplers:MultiJitteredSampler2D",
        "//samplers:Sampler2D",
        "//utils/io:loadMeshes",
        "//utils:RGBColor",
        "//utils:Vec3",
    ],
)

cc_library(
    name = "Camera",
    srcs = ["Camera.cc"],
    hdrs = ["Camera.h"],
    deps = [
        ":Object",
        ":Ray",
        ":ViewPlane",
        "//samplers:Sampler2D",
        "//utils:Vec3",
    ]
)

cc_library(
    name = "GeometricLayout",
    srcs = ["GeometricLayout.cc"],
    hdrs = ["GeometricLayout.h"],
    deps = [
        ":Ray",
        "//geometricObjects:GeometricObject",
        "//utils:constants",
        "//utils:RGBColor",
        "//utils:ShadeRec",
        "//utils:utils",
    ]
)

cc_library(
    name = "Light",
    srcs = ["Light.cc"],
    hdrs = ["Light.h"],
    deps = [
        "//utils:RGBColor",
        "//utils:Vec3",
    ]
)

cc_library(
    name = "Object",
    srcs = ["Object.cc"],
    hdrs = ["Object.h"],
    deps = [
        "//utils:Vec3",
    ]
)

cc_library(
    name = "Ray",
    srcs = ["Ray.cc"],
    hdrs = ["Ray.h"],
    deps = [
        ":Object",
    ]
)

cc_library(
    name = "ViewPlane",
    srcs = ["ViewPlane.cc"],
    hdrs = ["ViewPlane.h"]
)

cc_library(
    name = "World",
    srcs = ["World.cc"],
    hdrs = ["World.h"],
    deps = [
        ":Camera",
        ":GeometricLayout",
        ":Light",
        "//geometricObjects:GeometricObject",
        "//samplers:Sampler2D",
        "//shaders:Shader",
        "//utils:constants"
    ]
)