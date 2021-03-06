#pragma once

#include "geometricObjects/GeometricObject.h"
#include "utils/RGBColor.h"
#include "utils/ShadeRec.h"
#include "Ray.h"
#include <vector>
#include <tuple>
#include <unordered_map>

enum LayoutType {
  LIST,
  GRID
};

class GeometricLayout {
public:
  GeometricLayout();
  GeometricLayout(std::vector<GeometricObject*>& geometricObjects);
  ~GeometricLayout() = default;
  virtual bool Hit(
    const Ray& ray, double& t, ShadeRec& sr,
    bool earlyStopping = false, double stopT = -1);

  // [TODO] make it more general, something like nested layout
  void AddObject(GeometricObject* objPtr);
  void DeleteObjects();
  std::vector<GeometricObject*> GetObjects() { return geometricObjects_; };

protected:
  // Update implicitly
  virtual void UpdateLayout();
  void ExtendLayoutBBox(const BBox& objBBox);

  bool layoutUpdated;
  BBox layoutBoundingBox_;
  std::vector<GeometricObject*> geometricObjects_;
};

class GridLayout : public GeometricLayout {
public:
  GridLayout();
  GridLayout(std::vector<GeometricObject*>& geometricObjects);
  ~GridLayout() = default;
  bool Hit(
    const Ray& ray, double& t, ShadeRec& sr,
    bool earlyStopping = false, double stopT = -1) override;
private:
  void UpdateLayout() override;
  bool HitOutlier(const Ray& ray, double& t, ShadeRec& sr);
  Vec3 CalculateFRatio(const Vec3& position);
  std::unordered_map<uint64_t, std::vector<GeometricObject*>> grids_;
  std::vector<std::vector<GeometricObject*>> gridList;
  std::vector<GeometricObject*> outliers_;
  Vec3 w_;
  Vec3 n_;
};