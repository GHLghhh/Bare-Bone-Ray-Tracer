#include "fBmNoiseTexture.h"
#include "../utils/CubicNoise.h"

fBmNoiseTexture::fBmNoiseTexture()
  : Material()
{
  noiseFunction_.reset(new CubicNoise());
}

fBmNoiseTexture::fBmNoiseTexture(RGBColor materialColor, double gain, double lacunarity)
  : Material(materialColor)
{
  noiseFunction_.reset(new CubicNoise(gain, lacunarity));
}

fBmNoiseTexture::fBmNoiseTexture(const fBmNoiseTexture& rhs)
{
  color = rhs.color;
  diffuseCoefficient = rhs.diffuseCoefficient;
  specularCoefficient = rhs.specularCoefficient;
  shininess = rhs.shininess;
  emittedColor = rhs.emittedColor;
  noiseFunction_.reset(new CubicNoise(*(CubicNoise*)(rhs.noiseFunction_.get())));
}

fBmNoiseTexture::~fBmNoiseTexture()
{
  noiseFunction_.reset();
}

RGBColor fBmNoiseTexture::GetColor(const Vec3& hitPosition)
{
  return color * noiseFunction_->ValueTurbulence(hitPosition);
}

// [TODO] check if it can be simplified since it is the same as base class
fBmNoiseTexture& fBmNoiseTexture::operator=(const fBmNoiseTexture& rhs)
{
  color = rhs.color;
  diffuseCoefficient = rhs.diffuseCoefficient;
  specularCoefficient = rhs.specularCoefficient;
  shininess = rhs.shininess;
  emittedColor = rhs.emittedColor;
  noiseFunction_.reset(new CubicNoise(*(CubicNoise*)(rhs.noiseFunction_.get())));
  return *this;
}