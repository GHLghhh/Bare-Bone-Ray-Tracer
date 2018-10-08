#include "Light.h"

Light::Light(Vec3 spacialParameter, RGBColor color, bool shadow)
{
  param_ = spacialParameter;
  color_ = color;
  shadow_ = shadow;
}

DirectionalLight::DirectionalLight(
  Vec3 lightDirection, RGBColor color, bool shadow)
  : Light(lightDirection / lightDirection.Length(), color, shadow)
{
}

Vec3 DirectionalLight::ToLightDirection(const Vec3& hitPoint) const
{
  return param_ * -1;
}

PointLight::PointLight(Vec3 lightPosition, RGBColor color, bool shadow)
  : Light(lightPosition, color, shadow)
{
}

Vec3 PointLight::ToLightDirection(const Vec3& hitPoint) const
{
  return (param_ - hitPoint);
}