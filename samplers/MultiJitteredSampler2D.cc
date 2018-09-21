#include "MultiJitteredSampler2D.h"
#include <algorithm>
#include <random>

MultiJitteredSampler2D::MultiJitteredSampler2D()
  : Sampler2D()
{
  invColSquare = 1;
  invRowSquare = 1;
}

MultiJitteredSampler2D::MultiJitteredSampler2D(
  int numSamplesPerRow, int numSamplesPerCol)
  : Sampler2D(numSamplesPerRow, numSamplesPerCol)
{
  invColSquare = 1.0 / (numSamplesPerCol * numSamplesPerCol);
  invRowSquare = 1.0 / (numSamplesPerRow * numSamplesPerRow);
}

std::vector<Sample2D> MultiJitteredSampler2D::GenerateSample() const
{
  // (x,y) position index in terms of fine grid
  // according to N-Rooks concept
  std::vector<Sample2D> fineGridPosition;
  for (int x = 0; x < numSamplesPerRow_; x++) {
    for (int y = 0; y < numSamplesPerCol_; y++) {
      fineGridPosition.push_back(std::make_pair((double)x, (double)y));
    }
  }
  std::random_device rd;
  std::mt19937 g(rd());
  std::uniform_real_distribution<> dis(0, 1);
  std::shuffle(fineGridPosition.begin(), fineGridPosition.end(), g);

  std::vector<Sample2D> res;
  for (int x = 0; x < numSamplesPerRow_; x++) {
    for (int y = 0; y < numSamplesPerCol_; y++) {
      Sample2D fineGridPair = fineGridPosition[x * numSamplesPerCol_ + y];
      res.push_back(std::make_pair(
          (x + fineGridPair.first * invRowSquare + dis(g)) / numSamplesPerRow_,
          (y + fineGridPair.second * invColSquare + dis(g)) / numSamplesPerCol_));
    }
  }
  return res;
}