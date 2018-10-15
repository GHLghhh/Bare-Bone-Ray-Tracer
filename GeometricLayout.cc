#include "GeometricLayout.h"

GeometricLayout::GeometricLayout()
  : layoutUpdated(false)
{
}

void GeometricLayout::TransferFrom(GeometricLayout& rhs)
{
  layoutUpdated = false;
  geometricObjects_ = rhs.geometricObjects_;
  UpdateLayout();
}

void GeometricLayout::AddObject(GeometricObject* objPtr)
{
  geometricObjects_.push_back(objPtr);
  layoutUpdated = false;
}

bool GeometricLayout::Hit(
  const Ray& ray, double& t, ShadeRec& sr, bool earlyStopping, double stopT)
{
  UpdateLayout();
  // [TODO] eyePosition assignment is wierd
  t = -1;
  for (GeometricObject* objPtr : geometricObjects_) {
    double tTemp = t;
    ShadeRec srTemp(sr.eyePosition);
    // Hit
    if (objPtr->Hit(ray, tTemp, srTemp)) {
      // Check if closer
      if (t == -1 || tTemp < t) {
        t = tTemp;
        sr = srTemp;
        
        // check early stopping (for shadow ray)
        if (earlyStopping && (stopT == -1 || t < stopT)) {
          return true;
        }
      }
    }
  }
  return (t != -1);
}

void GeometricLayout::UpdateLayout()
{
  if (!layoutUpdated) {
    // Update layout here since geometric objects are changed
    layoutUpdated = true;
  }
}

bool GridLayout::Hit(
  const Ray& ray, double& t, ShadeRec& sr, bool earlyStopping, double stopT)
{
  UpdateLayout();
  // [TODO] eyePosition assignment is wierd
  t = -1;
  for (GeometricObject* objPtr : geometricObjects_) {
    double tTemp = t;
    ShadeRec srTemp(sr.eyePosition);
    // Hit
    if (objPtr->Hit(ray, tTemp, srTemp)) {
      // Check if closer
      if (t == -1 || tTemp < t) {
        t = tTemp;
        sr = srTemp;
        
        // check early stopping (for shadow ray)
        if (earlyStopping && (stopT == -1 || t < stopT)) {
          return true;
        }
      }
    }
  }
  return (t != -1);
}

void GridLayout::UpdateLayout()
{
  if (!layoutUpdated) {
    if (geometricObjects_.size() == 0) {
      return;
    }
    Vec3 minBBox = geometricObjects_[0]->boundingBox_.first;
    Vec3 maxBBox = geometricObjects_[0]->boundingBox_.second; 
    for (auto& objPtr : geometricObjects_) {
      // Do something
    }
    layoutUpdated = true;
  }
}