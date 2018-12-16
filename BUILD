cc_binary(
    name = "ray_tracer",
    deps = [
        "//src:RayTracer",
    ],
    linkopts = ["-lpthread"]
)

# should point to whatever is in test directory
cc_binary(
    name = "unit_test",
    deps = [
        "//src:RayTracer",
    ],
)