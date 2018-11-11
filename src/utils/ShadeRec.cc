#include "ShadeRec.h"

ShadeRec::ShadeRec(const Vec3& eyePosition)
  : objectHit(nullptr), diffuseCoefficient(0.9), specularCoefficient(0.9), shininess(10),
    eyePosition(eyePosition), emittedColor(0.0, 0.0, 0.0)
{
}

ShadeRec& ShadeRec::operator=(const ShadeRec& rhs)
{
  objectHit = rhs.objectHit;
  color = rhs.color;
  normal = rhs.normal;
  hitPosition = rhs.hitPosition;
  eyePosition = rhs.eyePosition;
  diffuseCoefficient = rhs.diffuseCoefficient;
  specularCoefficient = rhs.specularCoefficient;
  shininess = rhs.shininess;
  emittedColor = rhs.emittedColor;
  return *this;
}