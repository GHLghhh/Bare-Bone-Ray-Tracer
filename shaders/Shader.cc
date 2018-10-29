#include <iostream>
#include <math.h>

#include "Shader.h"

RGBColor Shader::Diffuse(const ShadeRec& sr, const Light& light)
{
  Vec3 lightDirection = light.ToLightDirection(sr.hitPosition);
  double angleCoefficient = Vec3::Dot(sr.normal, lightDirection);
  return (light.Color() * sr.color) * sr.diffuseCoefficient
    * ((angleCoefficient > 0) ? angleCoefficient : 0);
}

// [TODO] check why we need different "color" parameters for diffuse and specular
RGBColor Shader::Specular(const ShadeRec& sr, const Light& light)
{
  Vec3 lightDirection = light.ToLightDirection(sr.hitPosition);
  Vec3 reflectDirection =
    sr.normal * Vec3::Dot(sr.normal, lightDirection) * 2 - lightDirection;
  double reflectCoefficient = 
    Vec3::Dot(reflectDirection, (sr.eyePosition - sr.hitPosition).Unit());
  reflectCoefficient = (reflectCoefficient > 0) ? reflectCoefficient : 0;
  return (light.Color() * sr.color) * sr.specularCoefficient
    * pow(reflectCoefficient, sr.shininess);
}