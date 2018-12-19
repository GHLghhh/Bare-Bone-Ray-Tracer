#pragma once

#include <vector>

#include "../Object.h"
#include "../Ray.h"
#include "../materials/Material.h"
#include "../utils/ShadeRec.h"

// <minPosition, maxPosition>
using BBox = std::pair<Vec3, Vec3>;
using HitRangeElement = std::pair<double, ShadeRec>;

class GeometricObject : public Object {
public:
  // [TODO] should prohibit non-parametric constructor
  GeometricObject() = default;
  GeometricObject(Vec3 position, Material* material);
  ~GeometricObject() = default;
  virtual bool Hit(const Ray& ray, double& t, ShadeRec& s) = 0;
  // This function should only be overwritten by object that is "closed" (no edges)
  // Right now, the only implemented closed geometric object is sphere
  // This function will return the entering point and exiting point as time and shade record
  // which will be used for further computation in CSGObject class
  // [TODO] need to handle hit from inside correctly to fully support CSG
  virtual std::vector<HitRangeElement> HitRange(const Ray& ray);
  
  // [TODO] noting that bounding box as one point is special case
  // where the geometric object will always perform Hit check. i.e. plane
  BBox boundingBox_;
protected:
  virtual void FillShadeRec(const Ray& ray, const double t, ShadeRec& sr);
  Material* material_;
};