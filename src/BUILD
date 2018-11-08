package(default_visibility = ["//visibility:public"])

cc_library(
    name = "RayTracer",
    srcs = ["main.cc"],
    deps = [
        ":Ray",
        ":World",
        "//src/thirdParty:png",
        "//src/thirdParty:rgbapixel",
        "//src/geometricObjects:Plane",
        "//src/geometricObjects:Sphere",
        "//src/geometricObjects:Triangle",
        "//src/samplers:MultiJitteredSampler2D",
        "//src/samplers:Sampler2D",
        "//src/utils/io:loadMeshes",
        "//src/utils:RGBColor",
        "//src/utils:Vec3",
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
        "//src/samplers:Sampler2D",
        "//src/utils:Vec3",
    ]
)

cc_library(
    name = "GeometricLayout",
    srcs = ["GeometricLayout.cc"],
    hdrs = ["GeometricLayout.h"],
    deps = [
        ":Ray",
        "//src/geometricObjects:GeometricObject",
        "//src/utils:constants",
        "//src/utils:RGBColor",
        "//src/utils:ShadeRec",
        "//src/utils:utils",
    ]
)

cc_library(
    name = "Light",
    srcs = ["Light.cc"],
    hdrs = ["Light.h"],
    deps = [
        "//src/geometricObjects:Sphere",
        "//src/samplers:HemisphereSampler3D",
        "//src/utils:RGBColor",
        "//src/utils:Vec3",
    ]
)

cc_library(
    name = "Object",
    srcs = ["Object.cc"],
    hdrs = ["Object.h"],
    deps = [
        "//src/utils:Vec3",
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
        "//src/geometricObjects:GeometricObject",
        "//src/samplers:Sampler2D",
        "//src/shaders:Shader",
        "//src/utils:constants"
    ]
)