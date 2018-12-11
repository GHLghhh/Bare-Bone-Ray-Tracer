#pragma once

#include <memory>

#include "Material.h"
#include "../utils/RGBColor.h"
#include "../utils/Vec3.h"
#include "../utils/LatticeNoise.h"

// Store information about the object
// a.k.a. what is previously in ShadRec
class fBmNoiseTexture : public Material {
public:
  fBmNoiseTexture();
  fBmNoiseTexture(RGBColor materialColor, double gain = 0.5, double lacunarity = 2.0);
  fBmNoiseTexture(const fBmNoiseTexture& rhs);
  virtual ~fBmNoiseTexture();

  RGBColor GetColor(const Vec3& hitPosition) override;
  fBmNoiseTexture& operator= (const fBmNoiseTexture& rhs);

private:
  std::unique_ptr<LatticeNoise> noiseFunction_;
};