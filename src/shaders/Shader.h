#pragma once

#include "../Light.h"
#include "../utils/RGBColor.h"
#include "../utils/ShadeRec.h"

class Shader {
public:
  Shader() = default;
  // [TODO] abstract them into BRDF form
  static RGBColor Diffuse(const ShadeRec& sr, const Vec3& toLightDirection, const Light& light);
  static RGBColor Specular(const ShadeRec& sr, const Vec3& toLightDirection, const Light& light);

  static RGBColor PhongShadingModel(const ShadeRec& sr, const Vec3& toLightDirection, const Light& light);
};