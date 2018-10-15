#pragma once

#include "geometricObjects/GeometricObject.h"
#include "utils/RGBColor.h"
#include "utils/ShadeRec.h"
#include "Ray.h"
#include <vector>
#include <unordered_map>

enum LayoutType {
  LIST
};

class GeometricLayout {
public:
  GeometricLayout();
  void TransferFrom(GeometricLayout& rhs);
  virtual bool Hit(
    const Ray& ray, double& t, ShadeRec& sr,
    bool earlyStopping = false, double stopT = -1);

  // [TODO] make it more general, something like nested layout
  void AddObject(GeometricObject* objPtr);

protected:
  // Update implicitly
  virtual void UpdateLayout();
  // [TODO] BBox only used in actual accelerating structure
  // BBox layoutBoundingBox_;
  bool layoutUpdated;
  std::vector<GeometricObject*> geometricObjects_;
};

class GridLayout : public GeometricLayout {
public:
  GridLayout();
  bool Hit(
    const Ray& ray, double& t, ShadeRec& sr,
    bool earlyStopping = false, double stopT = -1);
private:
  void UpdateLayout();
  BBox layoutBoundingBox_;
  std::unordered_map<uintptr_t, std::vector<GeometricObject*>> grids;
};