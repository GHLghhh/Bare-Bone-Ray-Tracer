#include "Light.h"

Light::Light(Vec3 spacialParameter, RGBColor color)
{
  param_ = spacialParameter;
  color_ = color;
}

DirectionalLight::DirectionalLight(Vec3 lightDirection, RGBColor color)
  : Light(lightDirection / lightDirection.Length(), color)
{
}

Vec3 DirectionalLight::ToLightDirection(const Vec3& hitPoint) const
{
  return param_ * -1;
}

PointLight::PointLight(Vec3 lightPosition, RGBColor color)
  : Light(lightPosition, color)
{
}

Vec3 PointLight::ToLightDirection(const Vec3& hitPoint) const
{
  return (param_ - hitPoint);
}