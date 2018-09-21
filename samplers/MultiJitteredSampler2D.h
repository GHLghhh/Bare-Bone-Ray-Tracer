#pragma once

#include "Sampler2D.h"

class MultiJitteredSampler2D : public Sampler2D {
public:
  MultiJitteredSampler2D();
  MultiJitteredSampler2D(int numSamplesPerRow, int numSamplesPerCol);
  ~MultiJitteredSampler2D() = default;
  std::vector<Sample2D> GenerateSample() const override;
private:
  double invRowSquare;
  double invColSquare;
};