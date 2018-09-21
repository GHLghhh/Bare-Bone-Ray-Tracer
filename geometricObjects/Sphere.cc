#include "Sphere.h"
#include "../utils/constants.h"
#include <math.h>

Sphere::Sphere(Vec3 position, double radius, RGBColor color)
  : GeometricObject(position, color)
{
  // Sphere has no fixed normal, it depends on point position on sphere
  normal_ = Vec3(0,0,0);
  radius_ = radius;
}

bool Sphere::Hit(const Ray& ray, double& tMin, ShadeRec& sr)
{
  double a = Vec3::Dot(ray.Direction(), ray.Direction());
  double b = 2 * Vec3::Dot(ray.Position() - position_, ray.Direction());
  double c = Vec3::Dot(ray.Position() - position_, ray.Position() - position_)
    - radius_ * radius_;
  double d = b * b - 4 * a * c;
  // At least one interception point
  if (d >= 0) {
    double t1 = (-b - sqrt(d)) / (2 * a);
    double t2 = (-b + sqrt(d)) / (2 * a);
    double t = (t1 > kEPSILON ? t1 : (t2 > kEPSILON ? t2 : -1));
    if (t > kEPSILON) {
      tMin = t;
      sr.hitPosition = ray.Position() + ray.Direction() * t;
      sr.normal = sr.hitPosition - position_;
      sr.color = color_;
      return true;
    }
  }
  return false;
}