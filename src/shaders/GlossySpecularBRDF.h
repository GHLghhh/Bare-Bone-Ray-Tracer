#pragma once

#include <memory>

#include "BRDF.h"
#include "../samplers/HemisphereSampler3D.h"

class GlossySpecularBRDF : public BRDF {
public:
  /*
   * Default constructor that will set the specular reflection coefficient, kr_,
   * and specular color, cr_, to "black" (RGBColor(0.0, 0.0, 0.0)).
   */
  GlossySpecularBRDF();

  /*
   * Constructor that will set the specular reflection coefficient, kr_,
   * and specular color, cr_, based on the arguments.
   */
  GlossySpecularBRDF(RGBColor kr, RGBColor cr);

  /*
   * Destructor
   */
  ~GlossySpecularBRDF();

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
  RGBColor kr_;
  RGBColor cr_;
};