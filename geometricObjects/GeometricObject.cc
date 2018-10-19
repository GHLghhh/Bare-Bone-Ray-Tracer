#include "GeometricObject.h"

GeometricObject::GeometricObject(Vec3 position, RGBColor color)
  : Object(position), boundingBox_(position, position), color_(color)
{
}

void
GeometricObject::FillShadeRec(
  const Ray& ray, const double t, ShadeRec& sr)
{
  sr.color = color_;
  sr.hitPosition = ray.Position() + ray.Direction() * t;
}