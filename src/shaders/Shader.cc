#include <iostream>
#include <math.h>

#include "Shader.h"
#include "../materials/Material.h"

RGBColor Shader::Diffuse(const ShadeRec& sr, const Vec3& toLightDirection, const Light& light)
{
  Material* mat = sr.material;
  double angleCoefficient = Vec3::Dot(sr.normal, toLightDirection);
  return (light.Color() * mat->GetColor(sr.hitPosition)) * mat->diffuseCoefficient
    * ((angleCoefficient > 0) ? angleCoefficient : 0);
}

// [TODO] check why we need different "color" parameters for diffuse and specular
RGBColor Shader::Specular(const ShadeRec& sr, const Vec3& toLightDirection, const Light& light)
{
  Material* mat = sr.material;
  Vec3 reflectDirection =
    sr.normal * Vec3::Dot(sr.normal, toLightDirection) * 2 - toLightDirection;
  double reflectCoefficient = 
    Vec3::Dot(reflectDirection, (sr.eyePosition - sr.hitPosition).Unit());
  reflectCoefficient = (reflectCoefficient > 0) ? reflectCoefficient : 0;
  return (light.Color() * mat->GetColor(sr.hitPosition)) * mat->specularCoefficient
    * pow(reflectCoefficient, mat->shininess);
}

RGBColor Shader::PhongShadingModel(const ShadeRec& sr, const Vec3& toLightDirection, const Light& light)
{
  return Diffuse(sr, toLightDirection, light) + Specular(sr, toLightDirection, light);
}