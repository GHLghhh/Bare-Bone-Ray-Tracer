#include <iostream>

#include "Camera.h"

Camera::Camera(Vec3 position, Vec3 lookAt, Vec3 up)
  : Object(position)
{
  w_ = lookAt * -1;
  u_ = Vec3::Cross(up, w_);
  v_ = Vec3::Cross(w_, u_);
  
  w_ /= w_.Length();
  u_ /= u_.Length();
  v_ /= v_.Length();
}

PerspectiveCamera::PerspectiveCamera(Vec3 position, Vec3 lookAt, Vec3 up,
  double viewPlaneDepth)
  : Camera(position, lookAt, up), viewPlaneDepth_(viewPlaneDepth)
{
}

PixelRays PerspectiveCamera::CastRays(const Sampler2D& sampler, const ViewPlane& viewPlane)
{
  PixelRays res;
  for (int x = 0; x < viewPlane.HorizontalResolution(); x++) {
    res.push_back(std::vector<std::vector<Ray>>());
    for (int y = 0; y < viewPlane.VerticalResolution(); y++) {
      res.back().push_back(std::vector<Ray>());
      // [TODO] check if one batch of samples can be reused in multiple pixel
      std::vector<Sample2D> samples = sampler.GenerateSample();

      // Didn't include +0.5 here because this adjustment value is provided by sampler
      double pixelXPosition = (x - viewPlane.HorizontalResolution() / 2.0);
      double pixelYPosition = (y - viewPlane.VerticalResolution() / 2.0);
      for (Sample2D sample : samples) {
        // Apply adjustment value and
        // get the sample position in terms of world coordinate
        Vec3 pixelPoint = position_
          + w_ * (viewPlaneDepth_ * -1)
          + u_ * (pixelXPosition + sample.first) * viewPlane.PixelWidth()
          + v_ * (pixelYPosition + sample.second) * viewPlane.PixelHeight();
        
        res.back().back().push_back(Ray(position_, pixelPoint - position_));
      }
    }
  }
  return res;
}

OrthographicCamera::OrthographicCamera(Vec3 position, Vec3 lookAt, Vec3 up)
  : Camera(position, lookAt, up)
{
}

// [TODO] clean it up, difference between this and perspective is only how to generate rays
PixelRays OrthographicCamera::CastRays(const Sampler2D& sampler, const ViewPlane& viewPlane)
{
  PixelRays res;
  for (int x = 0; x < viewPlane.HorizontalResolution(); x++) {
    res.push_back(std::vector<std::vector<Ray>>());
    for (int y = 0; y < viewPlane.VerticalResolution(); y++) {
      res.back().push_back(std::vector<Ray>());
      // [TODO] check if one batch of samples can be reused in multiple pixel
      std::vector<Sample2D> samples = sampler.GenerateSample();

      // Didn't include +0.5 here because this adjustment value is provided by sampler
      double pixelXPosition = (x - viewPlane.HorizontalResolution() / 2.0);
      double pixelYPosition = (y - viewPlane.VerticalResolution() / 2.0);
      for (Sample2D sample : samples) {
        // Apply adjustment value and
        // get the sample position in terms of world coordinate
        Vec3 pixelPoint = position_
          + u_ * (pixelXPosition + sample.first) * viewPlane.PixelWidth()
          + v_ * (pixelYPosition + sample.second) * viewPlane.PixelHeight();
        
        res.back().back().push_back(Ray(pixelPoint, w_ * -1));
      }
    }
  }
  return res;
}