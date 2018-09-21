#include "Shader.h"

#include <iostream>

RGBColor Shader::Diffuse(const ShadeRec& sr, const Light& light)
{
  Vec3 lightDirection = light.ToLightDirection(sr.hitPosition);
  double angleCoefficient = Vec3::Dot(sr.normal, lightDirection);
  // [TODO] Find solution to avoid converting between classes
  return RGBColor((light.Color() * sr.color) * sr.diffuseCoefficient
    * ((angleCoefficient > 0) ? angleCoefficient : 0));
}