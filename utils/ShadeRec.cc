#include "ShadeRec.h"

ShadeRec::ShadeRec(const Vec3& eyePosition)
  : diffuseCoefficient(0.9), specularCoefficient(0.9), shininess(10),
    eyePosition(eyePosition)
{
}