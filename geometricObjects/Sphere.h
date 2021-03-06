#pragma once

#include "GeometricObject.h"

class Sphere : public GeometricObject {
public:
  Sphere() = default;
  Sphere(Vec3 position, double radius, RGBColor color);
  ~Sphere() = default;
  bool Hit(const Ray& ray, double& t, ShadeRec& s) override;
private:
  void FillShadeRec(const Ray& ray, const double t, ShadeRec& sr) override;
  double radius_;
};
