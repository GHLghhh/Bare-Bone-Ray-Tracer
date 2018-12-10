#include "PerfectSpecularBRDF.h"

#include "../utils/constants.h"

PerfectSpecularBRDF::PerfectSpecularBRDF()
  : kr_(RGBColor(0.0, 0.0, 0.0)), cr_(RGBColor(0.0, 0.0, 0.0))
{
}

PerfectSpecularBRDF::PerfectSpecularBRDF(RGBColor kr, RGBColor cr)
  : kr_(kr), cr_(cr)
{
}

PerfectSpecularBRDF::~PerfectSpecularBRDF()
{
}

RGBColor PerfectSpecularBRDF::f(const ShadeRec& sr, const Vec3& wi, const Vec3& wo)
{
  // Simply return black, assuming that the proper reflection rendering
  // is done in sample_f
  return RGBColor(0.0, 0.0, 0.0);
}

RGBColor PerfectSpecularBRDF::sample_f(const ShadeRec& sr, Vec3& wi, const Vec3& wo)
{
  // For perfect specular surface, the wi is always the reflected direction
  wi = sr.normal * Vec3::Dot(sr.normal, wo) * 2 - wo;

  // Cancel the cos term in BRDF rendering
  return (kr_ * cr_) / Vec3::Dot(sr.normal, wi);
}

RGBColor PerfectSpecularBRDF::rho(const ShadeRec& sr, const Vec3& wo)
{
  // [TODO] such property doesn't make sense in specular reflection?
  return RGBColor(0.0, 0.0, 0.0);
}