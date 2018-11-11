#include "Material.h"

Material::Material()
  : color(0.0, 0.0, 0.0), emittedColor(0.0, 0.0, 0.0),
    diffuseCoefficient(0.9), specularCoefficient(0.1), shininess(10)
{
}

Material::Material(RGBColor materialColor)
  : color(materialColor), emittedColor(0.0, 0.0, 0.0),
    diffuseCoefficient(0.9), specularCoefficient(0.1), shininess(10)
{
}

bool Material::SimpleLightSource(RGBColor lightColor)
{
  diffuseCoefficient = 0;
  specularCoefficient = 0;
  emittedColor = lightColor;
  return true;
}

Material& Material::operator=(const Material& rhs)
{
  color = rhs.color;
  diffuseCoefficient = rhs.diffuseCoefficient;
  specularCoefficient = rhs.specularCoefficient;
  shininess = rhs.shininess;
  emittedColor = rhs.emittedColor;
  return *this;
}