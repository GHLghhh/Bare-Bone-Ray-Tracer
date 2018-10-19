#include "GeometricLayout.h"
#include "utils/constants.h"
#include "utils/utils.h"
#include <math.h>
#include <iostream>

GeometricLayout::GeometricLayout()
  : layoutUpdated(false), layoutBoundingBox_(Vec3(0.0, 0.0, 0.0), Vec3(0.0, 0.0, 0.0))
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
  //std::cout << "update in plain" << std::endl;
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
    GridIdx directionSign({1, 1, 1});
    Vec3 tMin = Vec3(0.0, 0.0, 0.0);
    Vec3 tMax = Vec3(0.0, 0.0, 0.0);
    
    for (int i = 0; i < 3; i++) {
      if (invDirection[i] >= 0) {
        tMin[i] = (layoutBoundingBox_.first[i] - ray.Position()[i]) * invDirection[i];
        tMax[i] = (layoutBoundingBox_.second[i] - ray.Position()[i]) * invDirection[i];
      } else {
        directionSign[i] = 0;
        tMin[i] = (layoutBoundingBox_.second[i] - ray.Position()[i]) * invDirection[i];
        tMax[i] = (layoutBoundingBox_.first[i] - ray.Position()[i]) * invDirection[i];
      }
    }

    // make sure the box is hit
    if (tMax.x > 0 && tMax.y > 0 && tMax.z > 0) {
      bool isInside = (tMin.x < 0 && tMin.y < 0 && tMin.z < 0);
      Vec3 hitPoint = isInside ? ray.Position() : ray.PointAt(std::max(std::max(tMin.x, tMin.y), tMin.z));
      GridIdx startIdx = CalculateFRatio(hitPoint);
      Vec3 nextT(0.0, 0.0, 0.0);
      Vec3 deltaT = Vec3(
        w_.x / n_[0] * abs(invDirection.x),
        w_.y / n_[1] * abs(invDirection.y),
        w_.z / n_[2] * abs(invDirection.z));
      
      for (int i = 0; i < 3; i++) {
        nextT[i] = ((startIdx[i] + directionSign[i]) * w_[i] / n_[i] + layoutBoundingBox_.first[i] - hitPoint[i]) * invDirection[i];
      }
      if (!isInside) {
        nextT += std::max(std::max(tMin.x, tMin.y), tMin.z);
      }
      //std::cout << std::endl << n_ << " : " << w_ << " : " << nextT << std::endl;
      // traverse grid
      while (startIdx[0] < n_[0] && startIdx[1] < n_[1] && startIdx[2] < n_[2]
        && startIdx[0] >= 0 && startIdx[1] >= 0 && startIdx[2] >= 0) {
        // uint64_t idx = MortonCode((uint)startIdx.x, (uint)startIdx.y, (uint)startIdx.z);
        // auto itr = grids_.find(idx);
        //if (itr != grids_.end()) {
        //std::cout <<  "  " << startIdx; 
        if (gridList[startIdx[0] * n_[1] * n_[2] + startIdx[1] * n_[2] + startIdx[2]].size() != 0) {
          //std::cout << "  " << gridList[(int)startIdx.x * (int)n_.y * (int)n_.z + (int)startIdx.y * (int)n_.z + (int)startIdx.z].size();
          for (auto& objPtr : gridList[startIdx[0] * n_[1] * n_[2] + startIdx[1] * n_[2] + startIdx[2]]) {
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
          startIdx[0] += (directionSign[0] != 1) ? -1 : 1;
        } else if (nextT.y < nextT.z) {
          if (t != -1 && t <= nextT.y)
            return HitOutlier(ray, t, sr);
          nextT.y += deltaT.y;
          startIdx[1] += (directionSign[1] != 1) ? -1 : 1;
        } else {
          if (t != -1 && t <= nextT.z)
            return HitOutlier(ray, t, sr);
          nextT.z += deltaT.z;
          startIdx[2] += (directionSign[2] != 1) ? -1 : 1;
        }
      }
    }
  }
  return HitOutlier(ray, t, sr);
}

void GridLayout::UpdateLayout()
{
  if (!layoutUpdated) {
    // std::cout << "Update in grid" << std::endl;
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
      w_ = layoutBoundingBox_.second - layoutBoundingBox_.first;
      double s = pow(w_.x * w_.y * w_.z / geometricObjects_.size(), 1.0 / 3);
      n_.clear();
      for (int i = 0; i < 3; i++) {
        n_.push_back((size_t)(2 * w_[i] / s) + 1);
      }

      for (int idx = 0; idx < n_[0] * n_[1] * n_[2]; idx++) {
        gridList.push_back(std::vector<GeometricObject*>(0, nullptr));
      }

      // assign objects into grids
      for (auto& objPtr : geometricObjects_) {
        if (!(objPtr->boundingBox_.first == objPtr->boundingBox_.second)) {
          GridIdx minIdx = CalculateFRatio(objPtr->boundingBox_.first);
          GridIdx maxIdx = CalculateFRatio(objPtr->boundingBox_.second);
          //std::cout << "Obj at postion " << objPtr->Position() << " BBox: <" << minIdx << ", " << maxIdx << ">" << std::endl;
          for (size_t i = minIdx[0]; i <= maxIdx[0]; i++) {
            for (size_t j = minIdx[1]; j <= maxIdx[1]; j++) {
              for (size_t k = minIdx[2]; k <= maxIdx[2]; k++) {
                //std::cout << i << "," << j << "," << k << std::endl;
                gridList[i * n_[1] * n_[2] + j * n_[2] + k].push_back(objPtr);
                // uint64_t idx = MortonCode({i, j, k});
                // auto itr = grids_.find(idx);
                // if (itr == grids_.end()) {
                //   grids_.emplace(idx, std::vector<GeometricObject*>(1, objPtr));
                // } else {
                //   itr->second.push_back(objPtr);
                // }
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

GridLayout::GridIdx GridLayout::CalculateFRatio(const Vec3& position)
{
  GridIdx res;
  for (int i = 0; i < 3; i++) {
    double axisIdx = (position[i] - layoutBoundingBox_.first[i] - kEPSILON) / w_[i];
    // [TODO] examine why it is off (i.e. "hit" point far away from BBox)
    // may be fatal
    if (axisIdx < 0) {
      res.push_back(0);
      // if ((size_t)(axisIdx * n_[i]) > 0) {
      // std::cout << "Actual: " << ((position[i] - layoutBoundingBox_.first[i] - kEPSILON) / w_[i] * n_[i]) << std::endl;
      // std::cout << "Cast: " << (size_t)((position[i] - layoutBoundingBox_.first[i] - kEPSILON) / w_[i] * n_[i]) << std::endl;
      // std::cout << "Hit point: " << position << ", BBox: " << layoutBoundingBox_.first << ", idx: " << i << std::endl;
      // }
    } else if (axisIdx > 1) {
      res.push_back(n_[i]-1);
    } else {
      res.push_back((size_t)(axisIdx * n_[i]));
    }
  }
  return res;
}