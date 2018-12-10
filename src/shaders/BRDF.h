#pragma once

#include "../utils/RGBColor.h"
#include "../utils/ShadeRec.h"
#include "../utils/Vec3.h"

class BRDF {
public:
  /*
   * f() is used to compute the BRDF function value in direct illumination.
   * @param sr : shading record used to compute the BRDF function value.
   * @param wi : the incoming light direction (from hit point to light).
   * @param wo : the outgoing direction of the light after hitting the surface.
   * @return RGBColor : the ratio of the light being reflected in each channel, represented in RGBColor.
   */
  virtual RGBColor f(const ShadeRec& sr, const Vec3& wi, const Vec3& wo) = 0;

  /*
   * sample_f() is used to compute the BRDF function value in indirect illumination.
   * @param sr : shading record used to compute the BRDF function value.
   * @param wi : the incoming light direction (from hit point to light),
   *             in this function, it will be overwritten.
   *             User is supposed to used the overwritten value to further sample the incoming light color.
   * @param wo : the outgoing direction of the light after hitting the surface.
   * @return RGBColor : the ratio of the light being reflected in each channel, represented in RGBColor.
   * 
   * In contrast to direct illumination, we don't know the exact direction of incoming light in advanced,
   * so what we can do is to sample the direction based on important sampling of the BRDF
   * (assuming the probability distribution based on the BRDF property), and use the sampled direction
   * for computing the BRDF function value.
   */
  virtual RGBColor sample_f(const ShadeRec& sr, Vec3& wi, const Vec3& wo) = 0;

  /*
   * rho() returns the bi-hemispherical reflectance, the fraction of the total incident flux from the full hemisphere
   * that's reflected into the full hemisphere when the incident radiance is isotropic and spatially invariant.
   * @param sr : shading record used to compute the rho function value.
   * @param wo : the outgoing direction of the reflectance after hitting the surface, this function should be independent of wi and wo.
   * @return RGBColor : the ratio of the light being reflected in each channel, represented in RGBColor.
   */
  virtual RGBColor rho(const ShadeRec& sr, const Vec3& wo) = 0;
};