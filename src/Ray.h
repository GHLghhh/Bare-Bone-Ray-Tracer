#pragma once

#include "Object.h"

class Ray : public Object {
public:
  Ray() = default;
  Ray(Vec3 position, Vec3 direction);
  const Vec3& Direction() const { return direction_; };
  Vec3 PointAt(const double t) const;
private:
  Vec3 direction_;
};