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
  RGBColor color_;
  Vec3 normal_;
};