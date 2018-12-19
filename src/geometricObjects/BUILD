package(default_visibility = ["//visibility:public"])

cc_library(
    name = "GeometricObject",
    srcs = ["GeometricObject.cc"],
    hdrs = ["GeometricObject.h"],
    deps = [
        "//src:Object",
        "//src:Ray",
        "//src/materials:Material",
        "//src/utils:ShadeRec",
    ]
)

cc_library(
    name = "Plane",
    srcs = ["Plane.cc"],
    hdrs = ["Plane.h"],
    deps = [
        ":GeometricObject",
        "//src/materials:Material",
        "//src/utils:constants",
    ]
)

cc_library(
    name = "CSGObject",
    srcs = ["CSGObject.cc"],
    hdrs = ["CSGObject.h"],
    deps = [
        ":GeometricObject",
        "//src/materials:Material",
        "//src/utils:ShadeRec",
    ]
)

cc_library(
    name = "Sphere",
    srcs = ["Sphere.cc"],
    hdrs = ["Sphere.h"],
    deps = [
        ":GeometricObject",
        "//src/materials:Material",
        "//src/utils:constants",
    ]
)

cc_library(
    name = "Triangle",
    srcs = ["Triangle.cc"],
    hdrs = ["Triangle.h"],
    deps = [
        ":GeometricObject",
        ":Plane",
        "//src/materials:Material",
        "//src/utils:constants",
    ]
)