#pragma once

#include "RGBColor.h"
#include "Vec3.h"

#include "../materials/Material.h"

class ShadeRec {
public:
  ShadeRec() = default;
  // [TODO] put eye position for shaidng here?
  ShadeRec(const Vec3& eyePosition);

  ShadeRec& operator= (const ShadeRec& rhs);

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
  Material* material;
};