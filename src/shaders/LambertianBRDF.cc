#include "LambertianBRDF.h"

#include "../utils/constants.h"

LambertianBRDF::LambertianBRDF()
  : kd_(RGBColor(0.0, 0.0, 0.0)), cd_(RGBColor(0.0, 0.0, 0.0))
{
  samplerPtr_.reset(new HemisphereSampler3D(1));
}

LambertianBRDF::LambertianBRDF(RGBColor kd, RGBColor cd)
  : kd_(kd), cd_(cd)
{
}

LambertianBRDF::~LambertianBRDF()
{
  samplerPtr_.reset();
}

RGBColor LambertianBRDF::f(const ShadeRec& sr, const Vec3& wi, const Vec3& wo)
{
  return (kd_ * cd_) * kInvPi;
}

RGBColor LambertianBRDF::sample_f(const ShadeRec& sr, Vec3& wi, const Vec3& wo)
{
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

  return (kd_ * cd_) * kInvPi;
}

RGBColor LambertianBRDF::rho(const ShadeRec& sr, const Vec3& wo)
{
  return (kd_ * cd_);
}