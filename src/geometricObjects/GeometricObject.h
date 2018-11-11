#pragma once

#include "../Object.h"
#include "../Ray.h"
#include "../materials/Material.h"
#include "../utils/ShadeRec.h"

// <minPosition, maxPosition>
using BBox = std::pair<Vec3, Vec3>;

class GeometricObject : public Object {
public:
  GeometricObject() = default;
  GeometricObject(Vec3 position, Material* material);
  ~GeometricObject() = default;
  virtual bool Hit(const Ray& ray, double& t, ShadeRec& s) = 0;
  
  // [TODO] noting that bounding box as one point is special case
  // where the geometric object will always perform Hit check. i.e. plane
  BBox boundingBox_;
protected:
  virtual void FillShadeRec(const Ray& ray, const double t, ShadeRec& sr);
  Material* material_;
};