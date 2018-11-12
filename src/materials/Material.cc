#include "Material.h"

Material::Material()
  : color(0.0, 0.0, 0.0), emittedColor(0.0, 0.0, 0.0),
    diffuseCoefficient(0.9), specularCoefficient(0.0), transmissiveCoefficient(0.0),
    indexOfRefraction(1.0), shininess(10)
{
}

Material::Material(RGBColor materialColor)
  : color(materialColor), emittedColor(0.0, 0.0, 0.0),
    diffuseCoefficient(0.9), specularCoefficient(0.0), transmissiveCoefficient(0.0),
    indexOfRefraction(1.0), shininess(10)
{
}

bool Material::SimpleMirror()
{
  diffuseCoefficient = 0.0;
  specularCoefficient = 1.0;
  color = RGBColor(1.0, 1.0, 1.0);
  return true;
}

bool Material::SimpleTransparentMaterial()
{
  diffuseCoefficient = 0.0;
  specularCoefficient = 0.1;
  transmissiveCoefficient = 0.9;
  indexOfRefraction = 1.31;
  color = RGBColor(1.0, 1.0, 1.0);
  return true;
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