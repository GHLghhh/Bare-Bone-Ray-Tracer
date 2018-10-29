#include "ShadeRec.h"

ShadeRec::ShadeRec(const Vec3& eyePosition)
  : diffuseCoefficient(0.9), specularCoefficient(0.9), shininess(10),
    eyePosition(eyePosition)
{
}

ShadeRec& ShadeRec::operator=(const ShadeRec& rhs)
{
  color = rhs.color;
  normal = rhs.normal;
  hitPosition = rhs.hitPosition;
  eyePosition = rhs.eyePosition;
  diffuseCoefficient = rhs.diffuseCoefficient;
  specularCoefficient = rhs.specularCoefficient;
  shininess = rhs.shininess;
  return *this;
}