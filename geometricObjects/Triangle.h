#pragma once

#include "Plane.h"

class Triangle : public Plane {
public:
  Triangle() = default;
  Triangle(Vec3 point1, Vec3 point2, Vec3 point3, RGBColor color);
  ~Triangle() = default;
  bool Hit(const Ray& ray, double& t, ShadeRec& s) override;
private:
  bool IsInTriangle(Vec3 point);

  Vec3 point1_;
  Vec3 point2_;
  Vec3 point3_;
};