#include "Checker3D.h"

Checker3D::Checker3D()
  : Material()
{
  complementalColor = RGBColor(1.0, 1.0, 1.0) - color;
  // [TODO] hard coded for now, should be able to adjust it
  checkerSize = 0.3;
}

Checker3D::Checker3D(RGBColor materialColor)
  : Material(materialColor)
{
  complementalColor = RGBColor(1.0, 1.0, 1.0) - color;
  checkerSize = 0.3;
}

RGBColor Checker3D::GetColor(const Vec3& hitPosition)
{
  double x = hitPosition.x;
  double y = hitPosition.y;
  double z = hitPosition.z;
  if (((int)(x/checkerSize) + (int)(y/checkerSize) + (int)(z/checkerSize))
       % 2 == 0) {
    return color;
  } else {
    return complementalColor;
  }
}

// [TODO] check if it can be simplified since it is the same as base class
Checker3D& Checker3D::operator=(const Checker3D& rhs)
{
  color = rhs.color;
  diffuseCoefficient = rhs.diffuseCoefficient;
  specularCoefficient = rhs.specularCoefficient;
  shininess = rhs.shininess;
  emittedColor = rhs.emittedColor;
  return *this;
}