#pragma once

#include "Vec3.h"

class RGBColor : public Vec3 {
public:
  RGBColor() = default;
  RGBColor(double red, double green, double blue);
  RGBColor(const RGBColor& rhs);
  RGBColor(const Vec3& rhs);
  RGBColor To8Bit() const;
  RGBColor MaxToOne() const;
private:
  RGBColor(int red, int green, int blue);
};