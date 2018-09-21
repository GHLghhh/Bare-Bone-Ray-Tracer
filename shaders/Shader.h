#pragma once

#include "../utils/ShadeRec.h"
#include "../utils/RGBColor.h"
#include "../Light.h"

class Shader {
public:
  Shader() = default;
  static RGBColor Diffuse(const ShadeRec& sr, const Light& light);
};