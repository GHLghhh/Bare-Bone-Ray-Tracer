#include "Triangle.h"
#include "GeometricObject.h"
#include <iostream>

Triangle::Triangle(Vec3 point1, Vec3 point2, Vec3 point3, RGBColor color)
  : Plane(point2, Vec3::Cross(point3 - point2, point1 - point2), color)
{
  point1_ = point1;
  point2_ = point2;
  point3_ = point3;
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
  Vec3 cross1 = Vec3::Cross((point1_ - point), (point2_ - point));
  Vec3 cross2 = Vec3::Cross((point2_ - point), (point3_ - point));
  Vec3 cross3 = Vec3::Cross((point3_ - point), (point1_ - point));
  return (Vec3::Dot(cross1, cross2) >= 0 && Vec3::Dot(cross1, cross3) >= 0);
}