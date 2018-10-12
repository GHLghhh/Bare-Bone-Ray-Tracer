#pragma once

#include "../Object.h"
#include "../utils/RGBColor.h"
#include "../utils/ShadeRec.h"
#include "../Ray.h"

class GeometricObject : public Object {
public:
  GeometricObject() = default;
  GeometricObject(Vec3 position, RGBColor color);
  ~GeometricObject() = default;
  virtual bool Hit(const Ray& ray, double& t, ShadeRec& s) = 0;
protected:
  virtual void FillShadeRec(const Ray& ray, const double t, ShadeRec& sr);
  RGBColor color_;
};