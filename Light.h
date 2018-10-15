#pragma once

#include "utils/Vec3.h"
#include "utils/RGBColor.h"

class Light {
public:
  Light(Vec3 spacialParameter, RGBColor color, bool shadow = true);
  RGBColor Color() const { return color_ ;};
  virtual Vec3 ToLightDirection(const Vec3& hitPoint) const = 0;
  virtual double ToLightTime(const Vec3& hitPoint) const = 0;
protected:
  Vec3 param_;
  RGBColor color_;
  // For fast rendering.
  // If false, then not going to simulate shadow for this light source
  bool shadow_;
};

class DirectionalLight : public Light {
public:
  DirectionalLight(Vec3 lightDirection, RGBColor color, bool shadow = true);
  Vec3 ToLightDirection(const Vec3& hitPoint) const override;
  double ToLightTime(const Vec3& hitPoint) const override;
};

class PointLight : public Light {
public:
  PointLight(Vec3 lightPosition, RGBColor color, bool shadow = true);
  Vec3 ToLightDirection(const Vec3& hitPoint) const override;
  double ToLightTime(const Vec3& hitPoint) const override;
};