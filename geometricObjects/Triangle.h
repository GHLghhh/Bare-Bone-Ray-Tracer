#pragma once

#include "Plane.h"

class Triangle : public Plane {
public:
  Triangle() = default;
  Triangle(Vec3 point0, Vec3 point1, Vec3 point2, RGBColor color);
  ~Triangle() = default;
  bool Hit(const Ray& ray, double& t, ShadeRec& s) override;

  friend std::ostream& operator<< (std::ostream& out, const Triangle& obj); 
private:
  bool IsInTriangle(Vec3 point);
  void FillShadeRec(const Ray& ray, const double t, ShadeRec& sr) override;

  Vec3 point0_;
  Vec3 point1_;
  Vec3 point2_;
  Vec3 pointNormal0_;
  Vec3 pointNormal1_;
  Vec3 pointNormal2_;
};