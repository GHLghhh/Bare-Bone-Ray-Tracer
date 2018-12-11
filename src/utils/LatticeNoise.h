#pragma once

#include "Vec3.h"

const int kTableSize = 256;
const int kTableMask = kTableSize - 1;
const int seedValue = 253;

class LatticeNoise {
public:
  LatticeNoise();
  LatticeNoise(double gain, double lacunarity);
  LatticeNoise(const LatticeNoise& rhs);
  virtual ~LatticeNoise();

  virtual double ValueNoise(const Vec3& position) const = 0;
  double ValuefBm(const Vec3& position);
  double ValueTurbulence(const Vec3& position);

protected:
  int index(int i, int j, int k) const;

  int permutationTable_[kTableSize];
  double valueTable_[kTableSize];
  double gain_;
  double lacunarity_;
  double numOctave_;
  double fsMax_;

private:
  void InitValueTable(int seed);
};