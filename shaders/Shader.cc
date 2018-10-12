#include "Shader.h"

#include <iostream>
#include <math.h>

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
  double reflectCoefficient = pow(
    Vec3::Dot(reflectDirection, sr.eyePosition - sr.hitPosition),
    sr.shininess);
  return (light.Color() * sr.color) * sr.specularCoefficient
    * ((reflectCoefficient > 0) ? reflectCoefficient : 0);
}