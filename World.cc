#include "World.h"

#include <iostream>
#include "shaders/Shader.h"

World::World() : cameraPtr_(nullptr), viewPlanePtr_(nullptr), samplerPtr_(nullptr)
{
}

void World::SetCamera(Camera* objPtr)
{
  cameraPtr_ = objPtr;
}

void World::SetViewPlane(ViewPlane* objPtr)
{
  viewPlanePtr_ = objPtr;
}

void World::SetSampler(Sampler2D* objPtr)
{
  samplerPtr_ = objPtr;
}

void World::AddGeometricObject(GeometricObject* objPtr)
{
  geometricObjects_.push_back(objPtr);
}

void World::AddLightSource(Light* objPtr)
{
  lights_.push_back(objPtr);
}

Scene World::Render()
{
  Scene res;
  if (cameraPtr_ == nullptr || viewPlanePtr_ == nullptr || samplerPtr_ == nullptr) {
    // [TODO] error handling
    std::cout << "World not set up properly" << std::endl;
    return res;
  }
  PixelRays pixelRays = cameraPtr_->CastRays(*samplerPtr_, *viewPlanePtr_);
  for (auto& pixelsInRow : pixelRays) {
      res.push_back(std::vector<RGBColor>());
      for (auto& pixelsInRowColumn : pixelsInRow) {
          // Get proper color for each sample for the pixel and take average
          RGBColor resColor(0.0, 0.0, 0.0);
          for (Ray& ray : pixelsInRowColumn) {
              double t = -1;
              ShadeRec sr;
              for (GeometricObject* objPtr : geometricObjects_) {
                  double tTemp;
                  ShadeRec srTemp;
                  // Hit
                  if (objPtr->Hit(ray, tTemp, srTemp)) {
                    // Check if closer
                    if (t == -1 || tTemp < t) {
                      t = tTemp;
                      sr = srTemp;
                    }
                  }
              }
              // No hit, use background color for this sample
              if (t == -1) {
                resColor += backGroundColor;
              } else if (lights_.size() > 0) {
                // [TODO] here assuming only one light source
                // and haven't consider shadow yet.
                resColor += Shader::Diffuse(sr, *lights_[0]);
              }
          }
          res.back().push_back(resColor / pixelsInRowColumn.size());
      }
  }
  return res;
}