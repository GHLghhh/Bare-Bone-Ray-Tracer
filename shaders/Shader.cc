#include "Shader.h"

#include <iostream>

RGBColor Shader::Diffuse(const ShadeRec& sr, const Light& light)
{
  Vec3 lightDirection = light.ToLightDirection(sr.hitPosition);
  double angleCoefficient = Vec3::Dot(sr.normal, lightDirection);
  return (light.Color() * sr.color) * sr.diffuseCoefficient
    * ((angleCoefficient > 0) ? angleCoefficient : 0);
}

RGBColor Shader::Specular(const ShadeRec& sr, const Light& light)
{
  // [TODO] add correct implementation
  return RGBColor(0.0, 0.0, 0.0);
}