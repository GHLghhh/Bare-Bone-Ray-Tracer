#pragma once

#include "LatticeNoise.h"
#include "Vec3.h"

class CubicNoise : public LatticeNoise {
public:
  CubicNoise();
  CubicNoise(double gain, double lacunarity);
  CubicNoise(const CubicNoise& rhs);
  ~CubicNoise() = default;

  double ValueNoise(const Vec3& position) const override;
  
private:
  template<class T>
  T FourKnotSpline(const double x, const T knots[]) const;
};