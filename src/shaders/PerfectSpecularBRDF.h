#pragma once

#include <memory>

#include "BRDF.h"

class PerfectSpecularBRDF : public BRDF {
public:
  /*
   * Default constructor that will set the specular reflection coefficient, kr_,
   * and specular color, cr_, to "black" (RGBColor(0.0, 0.0, 0.0)).
   */
  PerfectSpecularBRDF();

  /*
   * Constructor that will set the specular reflection coefficient, kr_,
   * and specular color, cr_, based on the arguments.
   */
  PerfectSpecularBRDF(RGBColor kr, RGBColor cr);

  /*
   * Destructor
   */
  ~PerfectSpecularBRDF();

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
  RGBColor kr_;
  RGBColor cr_;
};