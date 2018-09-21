#pragma once

#include "GeometricObject.h"

class Plane : public GeometricObject {
public:
  Plane() = default;
  Plane(Vec3 position, Vec3 normal, RGBColor color);
  ~Plane() = default;
  virtual bool Hit(const Ray& ray, double& t, ShadeRec& s) override;
};