#pragma once

#include "GeometricObject.h"
#include "../materials/Material.h"

class Sphere : public GeometricObject {
public:
  Sphere() = default;
  Sphere(Vec3 position, double radius, Material* material);
  ~Sphere() = default;
  bool Hit(const Ray& ray, double& t, ShadeRec& s) override;
protected:
  double radius_;
  virtual void FillShadeRec(const Ray& ray, const double t, ShadeRec& sr) override;
};
