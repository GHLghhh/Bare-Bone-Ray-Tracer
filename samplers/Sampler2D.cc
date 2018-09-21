#include "Sampler2D.h"

Sampler2D::Sampler2D() : numSamplesPerRow_(1), numSamplesPerCol_(1)
{
}

Sampler2D::Sampler2D(int numSamplesPerRow, int numSamplesPerCol)
  : numSamplesPerRow_(numSamplesPerRow), numSamplesPerCol_(numSamplesPerCol)
{
}

std::vector<Sample2D> Sampler2D::GenerateSample() const
{
  std::vector<Sample2D> res;
  for (int x = 0; x < numSamplesPerRow_; x++) {
    for (int y = 0; y < numSamplesPerCol_; y++) {
      res.push_back(std::make_pair(
          (x + 0.5) / numSamplesPerRow_, (y + 0.5) / numSamplesPerCol_));
    }
  }
  return res;
}