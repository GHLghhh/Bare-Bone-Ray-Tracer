#include <iostream>
#include <math.h>

#include "Shader.h"

RGBColor Shader::Diffuse(const ShadeRec& sr, const Vec3& toLightDirection, const Light& light)
{
  double angleCoefficient = Vec3::Dot(sr.normal, toLightDirection);
  return (light.Color() * sr.color) * sr.diffuseCoefficient
    * ((angleCoefficient > 0) ? angleCoefficient : 0);
}

// [TODO] check why we need different "color" parameters for diffuse and specular
RGBColor Shader::Specular(const ShadeRec& sr, const Vec3& toLightDirection, const Light& light)
{
  Vec3 reflectDirection =
    sr.normal * Vec3::Dot(sr.normal, toLightDirection) * 2 - toLightDirection;
  double reflectCoefficient = 
    Vec3::Dot(reflectDirection, (sr.eyePosition - sr.hitPosition).Unit());
  reflectCoefficient = (reflectCoefficient > 0) ? reflectCoefficient : 0;
  return (light.Color() * sr.color) * sr.specularCoefficient
    * pow(reflectCoefficient, sr.shininess);
}

RGBColor Shader::PhongShadingModel(const ShadeRec& sr, const Vec3& toLightDirection, const Light& light)
{
  return Diffuse(sr, toLightDirection, light) + Specular(sr, toLightDirection, light);
}