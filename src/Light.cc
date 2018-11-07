#include "Light.h"

Light::Light(Vec3 spacialParameter, RGBColor color, bool shadow)
{
  param_ = spacialParameter;
  color_ = color;
  shadow_ = shadow;
}

DirectionalLight::DirectionalLight(
  Vec3 lightDirection, RGBColor color, bool shadow)
  : Light(lightDirection.Unit(), color, shadow)
{
}

std::vector<Vec3> DirectionalLight::ToLightDirection(const Vec3& hitPoint) const
{
  return std::vector<Vec3>(1, param_ * -1);
}

double DirectionalLight::ToLightTime(const Vec3& hitPoint) const
{
  return -1;
}

PointLight::PointLight(Vec3 lightPosition, RGBColor color, bool shadow)
  : Light(lightPosition, color, shadow)
{
}

std::vector<Vec3> PointLight::ToLightDirection(const Vec3& hitPoint) const
{
  return std::vector<Vec3>(1, (param_ - hitPoint).Unit());
}

double PointLight::ToLightTime(const Vec3& hitPoint) const
{
  return (param_ - hitPoint).Length();
}

AreaLight::AreaLight(Vec3 lightPosition, RGBColor color,
  bool shadow, bool useSampler)
  : Light(lightPosition, color, shadow)
{
  useSampler_ = useSampler;
}


SphereAreaLight::SphereAreaLight(Vec3 lightPosition, double radius, RGBColor color,
  bool shadow, bool useSampler)
  : AreaLight(lightPosition, color, shadow, useSampler), Sphere(lightPosition, radius, color)
{
}

std::vector<Vec3> SphereAreaLight::ToLightDirection(const Vec3& hitPoint) const
{
  return std::vector<Vec3>(1, Vec3());
}

double SphereAreaLight::ToLightTime(const Vec3& hitPoint) const
{
  return 0.0;
}