#pragma once

#include "GeometricObject.h"

enum CSGOperation {
  UNION,
  INTERSECTION,
  DIFFERENCE
};

// [TODO] For now, all inner geometric object are using the same material
class CSGObject : public GeometricObject {
public:
  CSGObject() = default;
  CSGObject(GeometricObject* leftOperand, GeometricObject* rightOperand, CSGOperation operation);
  ~CSGObject() = default;
  bool Hit(const Ray& ray, double& t, ShadeRec& s) override;
  // [TODO] Here using implicit assumption that
  // even index is entering point and odd index is exiting point
  std::vector<HitRangeElement> HitRange(const Ray& ray) override;
private:
  GeometricObject* leftOperand_;
  GeometricObject* rightOperand_;
  CSGOperation operation_;
};