#pragma once

#include "RGBColor.h"
#include "Vec3.h"

class ShadeRec {
public:
  ShadeRec() = default;
  // [TODO] put eye position for shaidng here?
  ShadeRec(const Vec3& eyePosition);

  ShadeRec& operator= (const ShadeRec& rhs);

  RGBColor color;
  Vec3 normal;
  Vec3 hitPosition;
  Vec3 eyePosition;
  // [TODO] This should be property of the hitted material
  // (now hard coded in ShadeRecctr)
  double diffuseCoefficient;
  double specularCoefficient;
  double shininess;
};