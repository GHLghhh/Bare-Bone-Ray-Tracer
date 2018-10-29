#include <iostream>
#include <math.h>

#include "GeometricLayout.h"
#include "utils/constants.h"
#include "utils/utils.h"

GeometricLayout::GeometricLayout()
  : layoutUpdated(false), layoutBoundingBox_(Vec3(0.0, 0.0, 0.0), Vec3(0.0, 0.0, 0.0))
{
}

GeometricLayout::GeometricLayout(std::vector<GeometricObject*>& geometricObjects)
  : layoutUpdated(false), geometricObjects_(geometricObjects)
{
}

void GeometricLayout::AddObject(GeometricObject* objPtr)
{
  geometricObjects_.push_back(objPtr);
  layoutUpdated = false;
}

void GeometricLayout::DeleteObjects()
{
  geometricObjects_.clear();
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

void GeometricLayout::ExtendLayoutBBox(const BBox& objBBox)
{
  Vec3& smallLocation = std::get<0>(layoutBoundingBox_);
  const Vec3& newSmall = std::get<0>(objBBox);
  
  Vec3& largeLocation = std::get<1>(layoutBoundingBox_);
  const Vec3& newLarge = std::get<1>(objBBox);
  for (size_t idx = 0; idx < 3; idx++) {
    // smaller location
    smallLocation[idx] = (smallLocation[idx] < newSmall[idx]) ? smallLocation[idx] : newSmall[idx];
    largeLocation[idx] = (largeLocation[idx] > newLarge[idx]) ? largeLocation[idx] : newLarge[idx];
  }
}

GridLayout::GridLayout()
  : GeometricLayout()
{
}

GridLayout::GridLayout(std::vector<GeometricObject*>& geometricObjects)
  : GeometricLayout(geometricObjects)
{
}

bool GridLayout::Hit(
  const Ray& ray, double& t, ShadeRec& sr, bool earlyStopping, double stopT)
{
  UpdateLayout();
  // [TODO] eyePosition assignment is wierd
  t = -1;
  // Grid layout doesn't have valid bounding box if there is nothing to be hit
  // except the outliers (objects without valid bounding box, i.e. plane)
  if (!(layoutBoundingBox_.first == layoutBoundingBox_.second)) {
    //std::cout << std::endl << "BBox: <" << layoutBoundingBox_.first << ", " << layoutBoundingBox_.second << ">" << std::endl;
    Vec3 invDirection = Vec3(
      1.0 / ray.Direction().x,
      1.0 / ray.Direction().y,
      1.0 / ray.Direction().z);
    Vec3 directionSign(1.0, 1.0, 1.0);
    Vec3 tMin = Vec3(0.0, 0.0, 0.0);
    Vec3 tMax = Vec3(0.0, 0.0, 0.0);
    //std::cout << "invDirection: " << invDirection << std::endl;
    if (invDirection.x >= 0) {
      tMin.x = (layoutBoundingBox_.first.x - ray.Position().x) * invDirection.x;
      tMax.x = (layoutBoundingBox_.second.x - ray.Position().x) * invDirection.x;
    } else {
      directionSign.x = 0;
      tMin.x = (layoutBoundingBox_.second.x - ray.Position().x) * invDirection.x;
      tMax.x = (layoutBoundingBox_.first.x - ray.Position().x) * invDirection.x;
    }
    if (invDirection.y >= 0) {
      tMin.y = (layoutBoundingBox_.first.y - ray.Position().y) * invDirection.y;
      tMax.y = (layoutBoundingBox_.second.y - ray.Position().y) * invDirection.y;
    } else {
      directionSign.y = 0;
      tMin.y = (layoutBoundingBox_.second.y - ray.Position().y) * invDirection.y;
      tMax.y = (layoutBoundingBox_.first.y - ray.Position().y) * invDirection.y;
    }
    if (invDirection.z >= 0) {
      tMin.z = (layoutBoundingBox_.first.z - ray.Position().z) * invDirection.z;
      tMax.z = (layoutBoundingBox_.second.z - ray.Position().z) * invDirection.z;
    } else {
      directionSign.z = 0;
      tMin.z = (layoutBoundingBox_.second.z - ray.Position().z) * invDirection.z;
      tMax.z = (layoutBoundingBox_.first.z - ray.Position().z) * invDirection.z;
    }
    // make sure the box is hit
    if (tMax.x > 0 && tMax.y > 0 && tMax.z > 0) {
      Vec3 startIdx;
      Vec3 nextT(0.0, 0.0, 0.0);
      Vec3 deltaT = Vec3(
        w_.x / n_.x * abs(invDirection.x),
        w_.y / n_.y * abs(invDirection.y),
        w_.z / n_.z * abs(invDirection.z));
      if (tMin.x < 0 && tMin.y < 0 && tMin.z < 0) {
        // ray origin is inside the box
        //std::cout << " should not hit: " << tMin << std::endl;
        startIdx = CalculateFRatio(ray.Position());
        nextT.x = ((startIdx.x + (int)directionSign.x) / n_.x * w_.x + layoutBoundingBox_.first.x - ray.Position().x) * invDirection.x;
        nextT.y = ((startIdx.y + (int)directionSign.y) / n_.y * w_.y + layoutBoundingBox_.first.y - ray.Position().y) * invDirection.y;
        nextT.z = ((startIdx.z + (int)directionSign.z) / n_.z * w_.z + layoutBoundingBox_.first.z - ray.Position().z) * invDirection.z;
      } else {
        // ray origin is outside the box
        double initT = std::max(std::max(tMin.x, tMin.y), tMin.z);
        Vec3 hitPoint = ray.PointAt(initT);
        startIdx = CalculateFRatio(hitPoint);
        nextT.x = initT + ((startIdx.x + (int)directionSign.x) / n_.x * w_.x + layoutBoundingBox_.first.x - hitPoint.x) * invDirection.x;
        nextT.y = initT + ((startIdx.y + (int)directionSign.y) / n_.y * w_.y + layoutBoundingBox_.first.y - hitPoint.y) * invDirection.y;
        nextT.z = initT + ((startIdx.z + (int)directionSign.z) / n_.z * w_.z + layoutBoundingBox_.first.z - hitPoint.z) * invDirection.z;
      }
      // traverse grid
      while ((int)startIdx.x < (int)n_.x && (int)startIdx.y < (int)n_.y && (int)startIdx.z < (int)n_.z
        && (int)startIdx.x >= 0 && (int)startIdx.y >= 0 && (int)startIdx.z >= 0) {
        if (gridList[(int)startIdx.x * (int)n_.y * (int)n_.z + (int)startIdx.y * (int)n_.z + (int)startIdx.z].size() != 0) {
          for (auto& objPtr : gridList[(int)startIdx.x * (int)n_.y * (int)n_.z + (int)startIdx.y * (int)n_.z + (int)startIdx.z]) {
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
        }
        if (nextT.x < nextT.y && nextT.x < nextT.z) {
          if (t != -1 && t <= nextT.x)
            return HitOutlier(ray, t, sr);
          nextT.x += deltaT.x;
          startIdx.x += (directionSign.x != 1.0) ? -1 : 1;
        } else if (nextT.y < nextT.z) {
          if (t != -1 && t <= nextT.y)
            return HitOutlier(ray, t, sr);
          nextT.y += deltaT.y;
          startIdx.y += (directionSign.y != 1.0) ? -1 : 1;
        } else {
          if (t != -1 && t <= nextT.z)
            return HitOutlier(ray, t, sr);
          nextT.z += deltaT.z;
          startIdx.z += (directionSign.z != 1.0) ? -1 : 1;
        }
      }
    }
  }
  return HitOutlier(ray, t, sr);
}

void GridLayout::UpdateLayout()
{
  if (!layoutUpdated) {
    // Recalculate bounding box and find outlier
    layoutBoundingBox_ = std::make_pair(Vec3(0.0, 0.0, 0.0), Vec3(0.0, 0.0, 0.0));
    grids_.clear();
    gridList.clear();
    outliers_.clear();
    if (geometricObjects_.size() == 0) {
      return;
    }
    layoutBoundingBox_ = std::make_pair(geometricObjects_[0]->boundingBox_.first, geometricObjects_[0]->boundingBox_.second);
    for (auto& objPtr : geometricObjects_) {
      // outlier
      if (objPtr->boundingBox_.first == objPtr->boundingBox_.second) {
        outliers_.push_back(objPtr);
      } else {
        ExtendLayoutBBox(objPtr->boundingBox_);
      }
    }

    if (!(layoutBoundingBox_.first == layoutBoundingBox_.second)) {
      for (int i = 0; i < 3; i++) {
        layoutBoundingBox_.first[i] -= kEPSILON;
        layoutBoundingBox_.second[i] += kEPSILON;
      }

      // calculate grid information
      // [TODO] make it a function? make 'm' a parameter
      double w_x = layoutBoundingBox_.second.x - layoutBoundingBox_.first.x;
      double w_y = layoutBoundingBox_.second.y - layoutBoundingBox_.first.y;
      double w_z = layoutBoundingBox_.second.z - layoutBoundingBox_.first.z;
      double s = pow(w_x * w_y * w_z / geometricObjects_.size(), 1.0 / 3);
      int n_x = (int)(2 * w_x / s) + 1;
      int n_y = (int)(2 * w_y / s) + 1;
      int n_z = (int)(2 * w_z / s) + 1;

      for (int idx = 0; idx < n_x * n_y * n_z; idx++) {
        gridList.push_back(std::vector<GeometricObject*>(0, nullptr));
      }

      // assign objects into grids
      w_ = Vec3(w_x, w_y, w_z);
      n_ = Vec3(n_x, n_y, n_z);
      // std::cout << "Layout BBox: " << w_ << std::endl;
      // std::cout << "Grids: " << n_ << std::endl;
      for (auto& objPtr : geometricObjects_) {
        if (!(objPtr->boundingBox_.first == objPtr->boundingBox_.second)) {
          Vec3 minIdx = CalculateFRatio(objPtr->boundingBox_.first);
          Vec3 maxIdx = CalculateFRatio(objPtr->boundingBox_.second);
          //std::cout << "Obj at postion " << objPtr->Position() << " BBox: <" << minIdx << ", " << maxIdx << ">" << std::endl;
          for (int i = (int)minIdx.x; i <= (int)maxIdx.x; i++) {
            for (int j = (int)minIdx.y; j <= (int)maxIdx.y; j++) {
              for (int k = (int)minIdx.z; k <= (int)maxIdx.z; k++) {
                // [TODO] use hash + Morton code for storage
                gridList[i * n_y * n_z + j * n_z + k].push_back(objPtr);
              }
            }
          }
        }
      }
    }
    layoutUpdated = true;
  }
}

bool GridLayout::HitOutlier(const Ray& ray, double& t, ShadeRec& sr)
{
  for (GeometricObject* objPtr : outliers_) {
    double tTemp = t;
    ShadeRec srTemp(sr.eyePosition);
    // Hit
    if (objPtr->Hit(ray, tTemp, srTemp)) {
      // Check if closer
      if (t == -1 || tTemp < t) {
        t = tTemp;
        sr = srTemp;
      }
    }
  }
  return (t != -1);
}

Vec3 GridLayout::CalculateFRatio(const Vec3& position)
{
  return Vec3(
    (int)((position.x - layoutBoundingBox_.first.x - kEPSILON) / w_.x * n_.x),
    (int)((position.y - layoutBoundingBox_.first.y - kEPSILON) / w_.y * n_.y),
    (int)((position.z - layoutBoundingBox_.first.z - kEPSILON) / w_.z * n_.z));
}