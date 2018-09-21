#include "Plane.h"
#include "../utils/constants.h"

Plane::Plane(Vec3 position, Vec3 normal, RGBColor color)
  : GeometricObject(position, color)
{
  normal_ = normal / normal.Length();
}

bool Plane::Hit(const Ray& ray, double& tMin, ShadeRec& sr)
{
  double t = Vec3::Dot(position_ - ray.Position(), normal_)
    / Vec3::Dot(ray.Direction(), normal_);
  if (t > kEPSILON) {
    tMin = t;
    sr.normal = normal_;
    sr.color = color_;
    sr.hitPosition = ray.Position() + ray.Direction() * t;
    return true;
  }
  return false;
}