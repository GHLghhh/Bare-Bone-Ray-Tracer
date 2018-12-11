#include "CubicNoise.h"
#include <iostream>

CubicNoise::CubicNoise()
  : LatticeNoise()
{
}

CubicNoise::CubicNoise(double gain, double lacunarity)
  : LatticeNoise(gain, lacunarity)
{
}

CubicNoise::CubicNoise(const CubicNoise& rhs)
  : LatticeNoise(rhs)
{
}

double CubicNoise::ValueNoise(const Vec3& position) const
{
  int ix, iy, iz;
  double dx, dy, dz;
  double xknots[4], yknots[4], zknots[4];

  ix = (int)position.x;
  dx = position.x - ix;
  iy = (int)position.y;
  dy = position.y - iy;
  iz = (int)position.z;
  dz = position.z - iz;

  for (int k = -1; k <= 2; k++) {
    for (int j = -1; j <= 2; j++) {
      for (int i = -1; i <= 2; i++) {
        xknots[i+1] = valueTable_[index(ix + i, iy + j, iz + k)];
      }
      yknots[j+1] = FourKnotSpline(dx, xknots);
    }
    zknots[k+1] = FourKnotSpline(dy, yknots);
  }
  double res = FourKnotSpline(dz, zknots);

  return ((res > 1.0) ? 1.0 : ((res < -1.0) ? -1.0 : res));
}

template<class T>
T CubicNoise::FourKnotSpline(const double x, const T knots[]) const
{
  T c3 = -0.5 * knots[0] + 1.5 * knots[1] - 1.5 * knots[2] + 0.5 * knots[3];
  T c2 = knots[0] - 2.5 * knots[1] + 2.0 * knots[2] - 0.5 * knots[3];
  T c1 = 0.5 * (-knots[0] + knots[2]);
  T c0 = knots[1];

  return (T((c3 * x + c2) * x + c1) * x + c0);
}