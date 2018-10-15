#include "World.h"

#include <iostream>
#include "shaders/Shader.h"
#include "utils/constants.h"

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

// [TODO] normalize color range in returning "Scene"
Scene World::Render()
{
  Scene res;
  if (cameraPtr_ == nullptr || viewPlanePtr_ == nullptr || samplerPtr_ == nullptr) {
    // [TODO] error handling
    std::cout << "World not set up properly" << std::endl;
    return res;
  }
  PixelRays pixelRays = cameraPtr_->CastRays(*samplerPtr_, *viewPlanePtr_);
  size_t totalPixel = pixelRays.size();
  for (auto& pixelsInRow : pixelRays) {
      res.push_back(std::vector<RGBColor>());
      for (auto& pixelsInRowColumn : pixelsInRow) {
          // Get proper color for each sample for the pixel and take average
          RGBColor resColor(0.0, 0.0, 0.0);
          for (Ray& ray : pixelsInRowColumn) {
              double t = -1;
              ShadeRec sr;
              for (GeometricObject* objPtr : geometricObjects_) {
                  double tTemp = t;
                  ShadeRec srTemp(cameraPtr_->Position());
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
              } else {
                // [TODO] ambient component and wrap everything into a shading model?
                for (Light* light : lights_) {
                  bool blocked = false;
                  double toLightTime = light->ToLightTime(sr.hitPosition);
                  for (GeometricObject* objPtr : geometricObjects_) {
                    double tTemp = -1;
                    ShadeRec srTemp;
                    Ray shadowRay(sr.hitPosition, light->ToLightDirection(sr.hitPosition));
                    if (objPtr->Hit(shadowRay, tTemp, srTemp)) {
                      // Check if the hit is valid
                      blocked = (tTemp > kEPSILON && (tTemp < toLightTime || toLightTime == -1));
                    }
                    if (blocked) {
                      break;
                    }
                  }
                  if (!blocked) {
                    resColor += Shader::Diffuse(sr, *light);
                    resColor += Shader::Specular(sr, *light);
                  }
                }
              } 
          }
          res.back().push_back(resColor / pixelsInRowColumn.size());
      }
      std::cout << "Rendering current scene: " << res.size() << "/" << totalPixel << std::endl;
  }
  return res;
}