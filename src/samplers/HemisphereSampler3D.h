#pragma once

#include "Sampler2D.h"
#include "../utils/Vec3.h"

class HemisphereSampler3D : public Sampler2D {
public:
  HemisphereSampler3D();
  HemisphereSampler3D(int numSamples);
  ~HemisphereSampler3D() = default;
  std::vector<Sample2D> GenerateSample() const override;
  std::vector<Vec3> GenerateSamplePoints() const;
private:
  int numSamples_;
};