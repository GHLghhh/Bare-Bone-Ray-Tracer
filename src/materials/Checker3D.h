#pragma once

#include "Material.h"
#include "../utils/RGBColor.h"
#include "../utils/Vec3.h"

// Store information about the object
// a.k.a. what is previously in ShadRec
class Checker3D : public Material {
public:
  Checker3D();
  Checker3D(RGBColor materialColor);
  ~Checker3D() = default;

  RGBColor GetColor(const Vec3& hitPosition) override;
  Checker3D& operator= (const Checker3D& rhs);

  RGBColor complementalColor;
  double checkerSize;
};