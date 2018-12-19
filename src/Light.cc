#include <cmath>
#include <random>

#include "Light.h"

Light::Light(Vec3 spacialParameter, RGBColor color, bool shadow)
{
  param_ = spacialParameter;
  color_ = color;
  shadow_ = shadow;
}

DirectionalLight::DirectionalLight(
  Vec3 lightDirection, RGBColor color, bool shadow)
  : Light(lightDirection.Unit(), color, shadow)
{
}

std::vector<ToLightRecord> DirectionalLight::ToLightRecords(const Vec3& hitPoint) const
{
  return std::vector<ToLightRecord>(1, std::make_pair(param_ * -1, -1));
}

PointLight::PointLight(Vec3 lightPosition, RGBColor color, bool shadow)
  : Light(lightPosition, color, shadow)
{
}

std::vector<ToLightRecord> PointLight::ToLightRecords(const Vec3& hitPoint) const
{
  return std::vector<ToLightRecord>(1, std::make_pair((param_ - hitPoint).Unit(), (param_ - hitPoint).Length()));
}

AreaLight::AreaLight(Vec3 lightPosition, RGBColor color,
  int numSamples, bool shadow)
  : Light(lightPosition, color, shadow)
{
  numSamples_ = numSamples;
  area_ = 1;
}

double AreaLight::InvPDF(const Vec3& lightPosition)
{
  return area_;
}


SphereAreaLight::SphereAreaLight(Vec3 lightPosition, double radius, Material* material,
  int numSamples, bool shadow)
  : AreaLight(lightPosition, material->emittedColor, numSamples, shadow), Sphere(lightPosition, radius, material)
{
  sampler_ = HemisphereSampler3D(numSamples_);
  area_ = 4 * M_PI * radius_ * radius_;
  hemisphereArea_ = area_ / 2;
}

std::vector<ToLightRecord> SphereAreaLight::ToLightRecords(const Vec3& hitPoint) const
{
  // Create u, v, w unit vector for mapping hemisphere samples on world coordinate
  Vec3 w = (hitPoint - position_).Unit();
  // Variable used for generating orthogonal coordinate
  Vec3 driftW = Vec3(
    Vec3::Dot(Vec3(cos(M_PI_4), -sin(M_PI_4), 0.0), w),
    Vec3::Dot(Vec3(sin(M_PI_4), cos(M_PI_4), 0.0), w),
    Vec3::Dot(Vec3(0.0, 0.0, 1.0), w));

  Vec3 u = Vec3::Cross(w, driftW).Unit();
  Vec3 v = Vec3::Cross(w, u).Unit();

  std::vector<Vec3> localCoordSamples = sampler_.GenerateSamplePoints();

  std::vector<ToLightRecord> res;
  for (Vec3& sample : localCoordSamples) {
    // use sphere center and radius to calculate the exact "light point"
    Vec3 sampledPoint = position_ + (u * sample.x + v * sample.y + w * sample.z).Unit() * radius_;
    res.push_back(std::make_pair(sampledPoint, (sampledPoint - hitPoint).Length()));
  }
  return res;
}

void SphereAreaLight::FillShadeRec(const Ray& ray, const double t, ShadeRec& sr)
{
  Sphere::FillShadeRec(ray, t, sr);
  sr.objectHit = this;
}

double SphereAreaLight::InvPDF(const Vec3& lightPosition)
{
  return hemisphereArea_;
}

Vec3 SphereAreaLight::Normal(const Vec3& lightPosition)
{
  return (lightPosition - position_).Unit();
}

TriangleAreaLight::TriangleAreaLight(
  Vec3 lightVertex0, Vec3 lightVertex1, Vec3 lightVertex2,
  Material* material, int numSamples, bool shadow)
  : AreaLight(lightVertex2, material->emittedColor, numSamples, shadow),
    Triangle(lightVertex0, lightVertex1, lightVertex2, material)
{
  area_ = Vec3::Cross((lightVertex2 - lightVertex1), (lightVertex0 - lightVertex1)).Length() / 2;
}

std::vector<ToLightRecord> TriangleAreaLight::ToLightRecords(const Vec3& hitPoint) const
{
  std::random_device rd;
  std::mt19937 g(rd());
  std::uniform_real_distribution<> dis(0, 1);

  std::vector<ToLightRecord> res;
  for (int i = 0; i < numSamples_; i++) {
    // Generate uniform sample on triangle
    // Take point0 as base point (origin)
    // Get 3 point locations in corresponding local frame
    // Sample and transform to "world" frame
    // http://mathworld.wolfram.com/TrianglePointPicking.html
    Vec3 localPoint1 = point1_ - point0_;
    Vec3 localPoint2 = point2_ - point0_;

    Vec3 sample = localPoint1 * dis(g) + localPoint2 * dis(g) + point0_;

    if (!IsInTriangle(sample)) {
      sample = point2_ + point1_ - sample;
    }

    res.push_back(std::make_pair(sample, (sample - hitPoint).Length()));
  }
  return res;
}

void TriangleAreaLight::FillShadeRec(const Ray& ray, const double t, ShadeRec& sr)
{
  Triangle::FillShadeRec(ray, t, sr);
  sr.objectHit = this;
}

Vec3 TriangleAreaLight::Normal(const Vec3& lightPosition)
{
  return normal_;
}