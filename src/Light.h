#pragma once

#include <vector>

#include "geometricObjects/Sphere.h"
#include "samplers/HemisphereSampler3D.h"
#include "utils/RGBColor.h"
#include "utils/Vec3.h"

// <Light position, time to light> for area lights
// <To light direction, time to light> for non-area lights
using ToLightRecord = std::pair<Vec3, double>;

class Light {
public:
  Light(Vec3 spacialParameter, RGBColor color, bool shadow = true);
  RGBColor Color() const { return color_ ;};
  void SetColor(RGBColor color) { color_ = color; };
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
  virtual Vec3 Normal(const Vec3& lightPosition) = 0;
  virtual double InvPDF(const Vec3& lightPosition) = 0;

protected:
  int numSamples_;
  double area_;
};

// [TODO] make material natural in area light
class SphereAreaLight : public AreaLight, public Sphere {
public:
  SphereAreaLight(Vec3 lightPosition, double radius, Material* material,
    int numSamples, bool shadow = true);
  std::vector<ToLightRecord> ToLightRecords(const Vec3& hitPoint) const override;
  Vec3 Normal(const Vec3& lightPosition) override;
  double InvPDF(const Vec3& lightPosition) override;
protected:
  void FillShadeRec(const Ray& ray, const double t, ShadeRec& sr) override;
private:
  HemisphereSampler3D sampler_;
  double hemisphereArea_;
};