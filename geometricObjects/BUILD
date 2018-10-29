package(default_visibility = ["//visibility:public"])

cc_library(
    name = "GeometricObject",
    srcs = ["GeometricObject.cc"],
    hdrs = ["GeometricObject.h"],
    deps = [
        "//:Object",
        "//:Ray",
        "//utils:RGBColor",
        "//utils:ShadeRec",
    ]
)

cc_library(
    name = "Plane",
    srcs = ["Plane.cc"],
    hdrs = ["Plane.h"],
    deps = [
        ":GeometricObject",
        "//utils:constants",
    ]
)

cc_library(
    name = "Sphere",
    srcs = ["Sphere.cc"],
    hdrs = ["Sphere.h"],
    deps = [
        ":GeometricObject",
        "//utils:constants",
    ]
)

cc_library(
    name = "Triangle",
    srcs = ["Triangle.cc"],
    hdrs = ["Triangle.h"],
    deps = [
        ":GeometricObject",
        ":Plane",
        "//utils:constants",
    ]
)