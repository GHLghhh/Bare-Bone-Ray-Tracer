#pragma once

#include "GeometricObject.h"

class Plane : public GeometricObject {
public:
  Plane() = default;
  Plane(Vec3 position, Vec3 normal, RGBColor color);
  ~Plane() = default;
  virtual bool Hit(const Ray& ray, double& t, ShadeRec& s) override;
protected:
  Vec3 normal_;
private:
  void FillShadeRec(const Ray& ray, const double t, ShadeRec& sr) override;
};