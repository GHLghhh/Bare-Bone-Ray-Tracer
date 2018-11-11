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
  // [TODO] check if there is a better way to put object
  //        why here: in current implementation, we need to check
  //          if the object being hit is the same area light
  //          when we check shadowing when area light is involved
  void* objectHit;
  double diffuseCoefficient;
  double specularCoefficient;
  double shininess;
  // For area light
  Vec3 emittedColor;
};