#include <random>
#include <cmath>
#include <iostream>

#include "LatticeNoise.h"

LatticeNoise::LatticeNoise()
  : gain_(0.5), lacunarity_(2.0), numOctave_(4)
{
  InitValueTable(seedValue);
  fsMax_ = (1.0 - pow(gain_, numOctave_)) / (1.0 - gain_);
}

LatticeNoise::LatticeNoise(const LatticeNoise& rhs)
{
  gain_ = rhs.gain_;
  lacunarity_ = rhs.lacunarity_;
  numOctave_ = rhs.numOctave_;
  fsMax_ = rhs.fsMax_;
  for (int i = 0; i < kTableSize; i++) {
    permutationTable_[i] = rhs.permutationTable_[i];
    valueTable_[i] = rhs.valueTable_[i];
  }
}

LatticeNoise::LatticeNoise(double gain, double lacunarity)
  : gain_(gain), lacunarity_(lacunarity), numOctave_(4)
{
  InitValueTable(seedValue);
  fsMax_ = (1.0 - pow(gain_, numOctave_)) / (1.0 - gain_);
}

LatticeNoise::~LatticeNoise()
{
}

double LatticeNoise::ValuefBm(const Vec3& position)
{
  double amplitude = 1.0;
  double frequency = 1.0;
  double res = 0.0;

  for (int i = 0; i < numOctave_; i++) {
    res += amplitude * ValueNoise(position * frequency);
    amplitude *= gain_;
    frequency *= lacunarity_;
  }

  return (res + fsMax_) / (2 * fsMax_);
}

double LatticeNoise::ValueTurbulence(const Vec3& position)
{
  double amplitude = 1.0;
  double frequency = 1.0;
  double res = 0.0;

  for (int i = 0; i < numOctave_; i++) {
    res += amplitude * fabs(ValueNoise(position * frequency));
    amplitude *= gain_;
    frequency *= lacunarity_;
  }

  return res / fsMax_;
}

int LatticeNoise::index(int i, int j, int k) const
{
  // [TODO] why permutationTable_ return out of range number without the last "%"
  return permutationTable_[
           (permutationTable_[i % kTableSize]
              + permutationTable_[j % kTableSize]
              + permutationTable_[k % kTableSize]) % kTableSize] % kTableSize;
}

void LatticeNoise::InitValueTable(int seed)
{
  std::mt19937 g(seed);
  std::uniform_real_distribution<> valueDis(-1, 1);
  std::uniform_real_distribution<> permuteDis(0, 1);
  for (int i = 0; i < kTableSize; i++) {
      valueTable_[i] = valueDis(g);
      permutationTable_[i] = ((int)(permuteDis(g) * kTableSize)) % kTableSize;
  }
}