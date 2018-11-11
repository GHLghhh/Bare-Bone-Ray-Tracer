#include "ShadeRec.h"

ShadeRec::ShadeRec(const Vec3& eyePosition)
  : eyePosition(eyePosition), objectHit(nullptr), material(nullptr)
{
}

ShadeRec& ShadeRec::operator=(const ShadeRec& rhs)
{
  objectHit = rhs.objectHit;
  normal = rhs.normal;
  hitPosition = rhs.hitPosition;
  eyePosition = rhs.eyePosition;
  material = rhs.material;
  return *this;
}