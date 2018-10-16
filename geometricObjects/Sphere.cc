#include "Sphere.h"
#include "../utils/constants.h"
#include <math.h>

Sphere::Sphere(Vec3 position, double radius, RGBColor color)
  : GeometricObject(position, color)
{
  radius_ = radius;
  boundingBox_.first.x = position_.x - radius_;
  boundingBox_.first.y = position_.y - radius_;
  boundingBox_.first.z = position_.z - radius_;
  boundingBox_.second.x = position_.x + radius_;
  boundingBox_.second.y = position_.y + radius_;
  boundingBox_.second.z = position_.z + radius_;
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
      FillShadeRec(ray, t, sr);
      return true;
    }
  }
  return false;
}

void Sphere::FillShadeRec(const Ray& ray, const double t, ShadeRec& sr)
{
  GeometricObject::FillShadeRec(ray, t, sr);
  sr.normal = (sr.hitPosition - position_).Unit();
}