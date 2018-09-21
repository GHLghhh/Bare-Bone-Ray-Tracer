#pragma once

#include <vector>
#include <utility> 

using Sample2D = std::pair<double, double>;

class Sampler2D {
public:
  Sampler2D();
  Sampler2D(int numSamplesPerRow, int numSamplesPerCol);
  ~Sampler2D() = default;
  virtual std::vector<Sample2D> GenerateSample() const;
protected:
  int numSamplesPerRow_;
  int numSamplesPerCol_;
};