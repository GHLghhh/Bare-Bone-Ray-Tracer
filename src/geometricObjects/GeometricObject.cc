#include "GeometricObject.h"

GeometricObject::GeometricObject(Vec3 position, Material* material)
  : Object(position), boundingBox_(position, position), material_(material)
{  
}

void
GeometricObject::FillShadeRec(
  const Ray& ray, const double t, ShadeRec& sr)
{
  sr.material = material_;
  sr.hitPosition = ray.Position() + ray.Direction() * t;
  sr.objectHit = this;
}