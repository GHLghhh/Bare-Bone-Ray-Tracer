#pragma once

#include <vector>
#include "Object.h"
#include "Ray.h"
#include "ViewPlane.h"
#include "samplers/Sampler2D.h"

using PixelRays = std::vector<std::vector<std::vector<Ray>>>;

class Camera : public Object {
public:
  Camera() = default;

  // [TODO] Currently assuming that lookAt and up vector are orthogonal
  // such that the Camera direction is determined without further calculation
  Camera(Vec3 position, Vec3 lookAt, Vec3 up);

  virtual PixelRays CastRays(const Sampler2D& sampler, const ViewPlane& viewPlane) = 0;

protected:
  Vec3 lookAt_;
  Vec3 up_;
  Vec3 right_;
};

class PerspectiveCamera : public Camera {
public:
  PerspectiveCamera() = default;

  PerspectiveCamera(Vec3 position, Vec3 lookAt, Vec3 up, double viewPlaneDepth);

  PixelRays CastRays(const Sampler2D& sampler, const ViewPlane& viewPlane) override;

private:
  double viewPlaneDepth_;
};

class OrthographicCamera : public Camera {
public:
  OrthographicCamera() = default;

  OrthographicCamera(Vec3 position, Vec3 lookAt, Vec3 up);

  PixelRays CastRays(const Sampler2D& sampler, const ViewPlane& viewPlane) override;
};