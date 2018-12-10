#pragma once

#include <memory>

#include "BRDF.h"
#include "../samplers/HemisphereSampler3D.h"

class LambertianBRDF : public BRDF {
public:
  /*
   * Default constructor that will set the diffuse reflection coefficient, kd_,
   * and diffuse color, cd_, to "black" (RGBColor(0.0, 0.0, 0.0)).
   */
  LambertianBRDF();

  /*
   * Constructor that will set the diffuse reflection coefficient, kd_,
   * and diffuse color, cd_, based on the arguments.
   */
  LambertianBRDF(RGBColor kd, RGBColor cd);

  /*
   * Destructor
   */
  ~LambertianBRDF();

  /*
   * @see BRDF::f()
   */
  RGBColor f(const ShadeRec& sr, const Vec3& wi, const Vec3& wo) override;

  /*
   * @see BRDF::sample_f()
   */
  RGBColor sample_f(const ShadeRec& sr, Vec3& wi, const Vec3& wo) override;

  /*
   * @see BRDF::rho()
   */
  RGBColor rho(const ShadeRec& sr, const Vec3& wo) override;

private:
  std::unique_ptr<HemisphereSampler3D> samplerPtr_;
  RGBColor kd_;
  RGBColor cd_;
};