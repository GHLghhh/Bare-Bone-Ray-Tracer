#include <iostream>

#include "World.h"
#include "shaders/Shader.h"
#include "utils/constants.h"

World::World()
  : cameraPtr_(nullptr), viewPlanePtr_(nullptr), samplerPtr_(nullptr),
    type_(LayoutType::LIST)
{
  geometricLayoutPtr_ = std::unique_ptr<GeometricLayout>(new GeometricLayout());
}

World::World(const World& rhs)
  : backGroundColor(rhs.backGroundColor),
    cameraPtr_(rhs.cameraPtr_), viewPlanePtr_(rhs.viewPlanePtr_),
    samplerPtr_(rhs.samplerPtr_), lights_(rhs.lights_), type_(rhs.type_)
{
  ConvertFromExistingLayout(type_, (rhs.geometricLayoutPtr_->GetObjects()));
}

World::~World()
{
  geometricLayoutPtr_.reset();
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

void World::SetGeometricLayoutType(LayoutType type)
{
  ConvertFromExistingLayout(type, geometricLayoutPtr_->GetObjects());
}

void World::ConvertFromExistingLayout(
  LayoutType type, std::vector<GeometricObject*> layoutObjs)
{
  switch (type) {
    case LayoutType::LIST:
      {
        geometricLayoutPtr_.reset(new GeometricLayout(layoutObjs));
      }
      break;
    case LayoutType::GRID:
      {
        geometricLayoutPtr_.reset(new GridLayout(layoutObjs));
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
  geometricLayoutPtr_->AddObject(objPtr);
}

void World::DiscardGeometricObjects()
{
  geometricLayoutPtr_->DeleteObjects();
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
  size_t totalPixel = pixelRays.size();
  for (auto& pixelsInRow : pixelRays) {
    res.push_back(std::vector<RGBColor>());
    for (auto& pixelsInRowColumn : pixelsInRow) {
        // Get proper color for each sample for the pixel and take average
        RGBColor resColor(0.0, 0.0, 0.0);
        for (Ray& ray : pixelsInRowColumn) {
          double t = -1;
          ShadeRec sr(cameraPtr_->Position());
          bool isHit = geometricLayoutPtr_->Hit(ray, t, sr);
          // No hit, use background color for this sample
          if (!isHit) {
            resColor += backGroundColor;
          } else {
            // [TODO] ambient component and wrap everything into a shading model?
            for (Light* light : lights_) {
              RGBColor unweightedResColor(0.0, 0.0, 0.0);
              double tTemp = -1;
              ShadeRec srTemp;
              std::vector<ToLightRecord> toLightRecords = 
                light->ToLightRecords(sr.hitPosition);
              for (ToLightRecord& toLightRecord : toLightRecords) {
                Ray shadowRay(sr.hitPosition, toLightRecord.first);
              
                bool isBlocked = geometricLayoutPtr_->Hit(
                  shadowRay, tTemp, srTemp, true, toLightRecord.second);
                if (!isBlocked) {
                  unweightedResColor += Shader::Diffuse(sr, toLightRecord.first, *light);
                  unweightedResColor += Shader::Specular(sr, toLightRecord.first, *light);
                }
              }
              resColor += (unweightedResColor / toLightRecords.size());
            }
          }
        }
        res.back().push_back(resColor / pixelsInRowColumn.size());
    }
    if (res.size() % (std::max((int)(totalPixel / 10),1)) == 0) {
      std::cout << "Rendering current scene: " << res.size() << "/" << totalPixel << std::endl;
    }
  }
  return res;
}