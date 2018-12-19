#include <algorithm>
#include <cmath>
#include <random>

#include "HemisphereSampler3D.h"

HemisphereSampler3D::HemisphereSampler3D()
{
  numSamples_ = 1;
}

HemisphereSampler3D::HemisphereSampler3D(int numSamples)
{
  numSamples_ = numSamples;
}

// This function is actually used to generate (r1, r2) pairs
// for calculating the sampled 3D positions
std::vector<Sample2D> HemisphereSampler3D::GenerateSample() const
{
  std::vector<Sample2D> uniformRandomSamples;

  std::random_device rd;
  std::mt19937 g(rd());
  std::uniform_real_distribution<> dis(0, 1);

  for (int idx = 0; idx < numSamples_; idx++) {
    uniformRandomSamples.push_back(std::make_pair(dis(g), dis(g)));
  }
  return uniformRandomSamples;
}

// Return 3D point position in local coordinate
std::vector<Vec3> HemisphereSampler3D::GenerateSamplePoints() const
{
  std::vector<Vec3> res;
  for (auto& sample_pair : GenerateSample()) {
    double cos_phi = cos(2.0 * M_PI * sample_pair.first);
    double sin_phi = sin(2.0 * M_PI * sample_pair.first);
    // [TODO] e = 1 for now
    double cos_theta = pow(sample_pair.second, 0.5 /* 1 / (e + 1)*/);
    double sin_theta = sqrt (1.0 - sample_pair.second /*cos_theta * cos_theta*/);
    double pu = sin_theta * cos_phi;
    double pv = sin_theta * sin_phi;
    double pw = cos_theta;
    res.push_back(Vec3(pu, pv, pw));
  }

  return res;
}