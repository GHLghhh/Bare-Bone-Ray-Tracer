#include "World.h"

#include <iostream>
#include "shaders/Shader.h"
#include "utils/constants.h"

World::World()
  : cameraPtr_(nullptr), viewPlanePtr_(nullptr), samplerPtr_(nullptr)
{
  geometricLayout_ = GeometricLayout();
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

// [TODO] hide the layout creation
void World::SetGeometricLayoutType(LayoutType type)
{
  GeometricLayout old = geometricLayout_;
  switch (type) {
    case LayoutType::LIST:
      {
        geometricLayout_ = GeometricLayout();
        geometricLayout_.TransferFrom(old);
      }
      break;
    default:
      {
        std::cout << "Unknown layout type" << std::endl;
      }
  }
}

void World::AddGeometricObject(GeometricObject* objPtr)
{
  geometricLayout_.AddObject(objPtr);
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
            ShadeRec sr(cameraPtr_->Position());
            bool isHit = geometricLayout_.Hit(ray, t, sr);
            // No hit, use background color for this sample
            if (!isHit) {
              resColor += backGroundColor;
            } else {
              // [TODO] ambient component and wrap everything into a shading model?
              for (Light* light : lights_) {
                double toLightTime = light->ToLightTime(sr.hitPosition);
                double tTemp = -1;
                ShadeRec srTemp;
                Ray shadowRay(sr.hitPosition, light->ToLightDirection(sr.hitPosition));

                bool isBlocked = geometricLayout_.Hit(
                  shadowRay, tTemp, srTemp, true, toLightTime);
                if (!isBlocked) {
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