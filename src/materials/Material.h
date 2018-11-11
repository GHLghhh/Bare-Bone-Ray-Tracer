#pragma once

#include "../utils/RGBColor.h"
#include "../utils/Vec3.h"

// Store information about the object
// a.k.a. what is previously in ShadRec
class Material {
public:
  Material();
  Material(RGBColor materialColor);
  ~Material() = default;

  bool SimpleLightSource(RGBColor lightColor);

  Material& operator= (const Material& rhs);

  // [TODO] This should be property of the hitted material
  // (now hard coded in ShadeRecctr)
  // [TODO] check if there is a better way to put object
  //        why here: in current implementation, we need to check
  //          if the object being hit is the same area light
  //          when we check shadowing when area light is involved
  void* objectHit;
  RGBColor color;
  // For area light
  RGBColor emittedColor;
  double diffuseCoefficient;
  double specularCoefficient;
  double shininess;
};