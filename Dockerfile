FROM ubuntu:16.04

# Install bazel
RUN apt-get update
RUN apt-get install -y --no-install-recommends \
        openjdk-8-jdk curl apt-transport-https
RUN echo "deb [arch=amd64] https://storage.googleapis.com/bazel-apt stable jdk1.8" | tee /etc/apt/sources.list.d/bazel.list
RUN curl https://bazel.build/bazel-release.pub.gpg | apt-key add -
RUN apt-get update
RUN apt-get -y --no-install-recommends install bazel

# Compile source
WORKDIR /workspace
COPY . .
# [TODO] fix this workaround: --incompatible_remove_native_http_archive=false
RUN bazel build --incompatible_remove_native_http_archive=false --cxxopt='-std=c++11' ray_tracer