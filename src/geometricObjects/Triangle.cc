#include <iostream>

#include "GeometricObject.h"
#include "Triangle.h"

Triangle::Triangle(Vec3 point0, Vec3 point1, Vec3 point2, RGBColor color)
  : Plane(point2, Vec3::Cross(point1 - point0, point2 - point0), color)
{
  point0_ = point0;
  point1_ = point1;
  point2_ = point2;
  pointNormal0_ = normal_;
  pointNormal1_ = normal_;
  pointNormal2_ = normal_;

  Vec3 first;
  Vec3 second;
  for (size_t idx = 0; idx < 3; idx++) {
    first[idx] = std::min(std::min(point0_[idx], point1_[idx]), point2_[idx]);
    second[idx] = std::max(std::max(point0_[idx], point1_[idx]), point2_[idx]);
  }
  boundingBox_ = std::make_pair(first, second);
}

bool Triangle::Hit(const Ray& ray, double& tMin, ShadeRec& sr)
{
  if (Plane::Hit(ray, tMin, sr)) {
    // Check if in triangle
    if (IsInTriangle(ray.PointAt(tMin))) {
      return true;
    }
  }
  return false;
}

bool Triangle::IsInTriangle(Vec3 point)
{
  Vec3 cross1 = Vec3::Cross((point0_ - point), (point1_ - point));
  Vec3 cross2 = Vec3::Cross((point1_ - point), (point2_ - point));
  Vec3 cross3 = Vec3::Cross((point2_ - point), (point0_ - point));
  return (Vec3::Dot(cross1, cross2) >= 0 && Vec3::Dot(cross1, cross3) >= 0);
}

void Triangle::FillShadeRec(const Ray& ray, const double t, ShadeRec& sr)
{
  GeometricObject::FillShadeRec(ray, t, sr);
  // compute Barycentric coordinates
  // Fill ShadeRec only when hit point is in triangle,
  // can assume Barycentric coordinates are > 0
  double a0 = Vec3::Cross((point1_ - sr.hitPosition), (point2_ - sr.hitPosition)).Length();
  double a1 = Vec3::Cross((point2_ - sr.hitPosition), (point0_ - sr.hitPosition)).Length();
  double a2 = Vec3::Cross((point0_ - sr.hitPosition), (point1_ - sr.hitPosition)).Length();
  double a = a0 + a1 + a2;
  sr.normal = (pointNormal0_ * (a0 / a) + pointNormal1_ * (a1 / a) + pointNormal2_ * (a2 / a)).Unit();
}

std::ostream& operator<< (std::ostream& out, const Triangle& obj)
{
    out << "[" << obj.point0_ << " " << obj.point1_ << " " << obj.point2_ << "]";
    return out;
}