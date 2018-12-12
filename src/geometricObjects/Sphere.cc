#include <math.h>

#include "Sphere.h"
#include "../utils/constants.h"

Sphere::Sphere(Vec3 position, double radius, Material* material)
  : GeometricObject(position, material)
{
  radius_ = radius;
  for (int i = 0; i < 3; i++) {
    boundingBox_.first[i] = position_[i] - radius_;
    boundingBox_.second[i] = position_[i] + radius_;
  }
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

std::vector<HitRangeElement> Sphere::HitRange(const Ray& ray)
{
  std::vector<HitRangeElement> res;
  double a = Vec3::Dot(ray.Direction(), ray.Direction());
  double b = 2 * Vec3::Dot(ray.Position() - position_, ray.Direction());
  double c = Vec3::Dot(ray.Position() - position_, ray.Position() - position_)
    - radius_ * radius_;
  double d = b * b - 4 * a * c;
  // Similar to Hit() but only care the case that there are at least two hit point
  // such that we have a range of "hitting object"
  if (d > 0) {
    double tSmall = (-b - sqrt(d)) / (2 * a);
    double tBig = (-b + sqrt(d)) / (2 * a);
    if (tSmall > kEPSILON && tBig > kEPSILON) {
      if (tSmall > tBig) {
        double tTemp = tBig;
        tBig = tSmall;
        tSmall = tTemp;
      }
      res.push_back(std::make_pair(tSmall, ShadeRec()));
      res.push_back(std::make_pair(tBig, ShadeRec()));
      FillShadeRec(ray, tSmall, res[0].second);
      FillShadeRec(ray, tBig, res[1].second);
    }
  }
  return res;
}

void Sphere::FillShadeRec(const Ray& ray, const double t, ShadeRec& sr)
{
  GeometricObject::FillShadeRec(ray, t, sr);
  sr.normal = (sr.hitPosition - position_).Unit();
  sr.objectHit = this;
}