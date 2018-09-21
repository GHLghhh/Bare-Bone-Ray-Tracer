#pragma once

#include "utils/Vec3.h"

class Object {
public:
  // Default constructor
  Object() = default;

  // Constructor for creating object at (x, y, z) in respect to world coordinate
  Object(Vec3 position);

  // Default destructor;
  ~Object() = default;

  const Vec3& Position() const { return position_; };
  
protected:
  Vec3 position_;
};
