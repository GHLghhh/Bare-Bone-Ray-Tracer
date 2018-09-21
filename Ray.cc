#include "Ray.h"

Ray::Ray(Vec3 position, Vec3 direction)
{
  position_ = position;
  direction_ = direction / direction.Length();
}

Vec3 Ray::PointAt(const double t) const
{
  return position_ + direction_ * t;
}