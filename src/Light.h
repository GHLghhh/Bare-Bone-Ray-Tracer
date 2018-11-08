#pragma once

#include <vector>

#include "geometricObjects/Sphere.h"
#include "samplers/HemisphereSampler3D.h"
#include "utils/RGBColor.h"
#include "utils/Vec3.h"

using ToLightRecord = std::pair<Vec3, double>;

class Light {
public:
  Light(Vec3 spacialParameter, RGBColor color, bool shadow = true);
  RGBColor Color() const { return color_ ;};
  virtual std::vector<ToLightRecord> ToLightRecords(const Vec3& hitPoint) const = 0;
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
  std::vector<ToLightRecord> ToLightRecords(const Vec3& hitPoint) const override;
};

class PointLight : public Light {
public:
  PointLight(Vec3 lightPosition, RGBColor color, bool shadow = true);
  std::vector<ToLightRecord> ToLightRecords(const Vec3& hitPoint) const override;
};

class AreaLight : public Light {
public:
  AreaLight(Vec3 lightPosition, RGBColor color,
    int numSamples, bool shadow = true);
  virtual std::vector<ToLightRecord> ToLightRecords(const Vec3& hitPoint) const = 0;

protected:
  int numSamples_;
};

class SphereAreaLight : public AreaLight, public Sphere {
public:
  SphereAreaLight(Vec3 lightPosition, double radius, RGBColor color,
    int numSamples, bool shadow = true);
  std::vector<ToLightRecord> ToLightRecords(const Vec3& hitPoint) const override;
private:
  HemisphereSampler3D sampler;
};