#include "GlossySpecularBRDF.h"

#include "../utils/constants.h"

GlossySpecularBRDF::GlossySpecularBRDF()
  : kr_(RGBColor(0.0, 0.0, 0.0)), cr_(RGBColor(0.0, 0.0, 0.0))
{
  // [TODO] Modify HemisphereSampler3D to be able to control concentration
  samplerPtr_.reset(new HemisphereSampler3D(1));
}

GlossySpecularBRDF::GlossySpecularBRDF(RGBColor kr, RGBColor cr)
  : kr_(kr), cr_(cr)
{
}

GlossySpecularBRDF::~GlossySpecularBRDF()
{
  samplerPtr_.reset();
}

RGBColor GlossySpecularBRDF::f(const ShadeRec& sr, const Vec3& wi, const Vec3& wo)
{
  // [TODO]
  return RGBColor(0.0, 0.0, 0.0);
}

RGBColor GlossySpecularBRDF::sample_f(const ShadeRec& sr, Vec3& wi, const Vec3& wo)
{
  // [TODO]
  // Create u, v, w unit vector for mapping hemisphere samples on world coordinate
  Vec3 w = sr.normal;

  // [TODO] abstract it into function
  w = w.Unit();
  // Rotate w to get a non-parallel vector to perform the cross product
  Vec3 driftW = Vec3(
    Vec3::Dot(Vec3(cos(M_PI_4), -sin(M_PI_4), 0.0), w),
    Vec3::Dot(Vec3(sin(M_PI_4), cos(M_PI_4), 0.0), w),
    Vec3::Dot(Vec3(0.0, 0.0, 1.0), w));

  Vec3 u = Vec3::Cross(w, driftW).Unit();
  Vec3 v = Vec3::Cross(w, u).Unit();

  Vec3 localsample = samplerPtr_->GenerateSamplePoints()[0];

  // Overwrite wi with the hemisphere sample
  wi = (u * localsample.x + v * localsample.y + w * localsample.z).Unit();

  return RGBColor(0.0, 0.0, 0.0);
}

RGBColor GlossySpecularBRDF::rho(const ShadeRec& sr, const Vec3& wo)
{
  // [TODO] such property doesn't make sense in specular reflection?
  return RGBColor(0.0, 0.0, 0.0);
}