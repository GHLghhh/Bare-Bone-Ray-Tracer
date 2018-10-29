#pragma once

#include <memory>
#include <vector>

#include "Camera.h"
#include "GeometricLayout.h"
#include "Light.h"
#include "geometricObjects/GeometricObject.h"
#include "samplers/Sampler2D.h"


using Scene = std::vector<std::vector<RGBColor>>;

class World {
public:
  World();
  World(const World& rhs);
  ~World();

  // [TODO] find a better way to manage objects
  // want main to be free of storing temporary objects
  void SetCamera(Camera* objPtr);
  void SetViewPlane(ViewPlane* objPtr);
  void SetSampler(Sampler2D* objPtr);
  void SetGeometricLayoutType(LayoutType type);

  void AddGeometricObject(GeometricObject* objPtr);
  void DiscardGeometricObjects();
  void AddLightSource(Light* objPtr);

  Scene Render();
private:
  void ConvertFromExistingLayout(
    LayoutType type, std::vector<GeometricObject*> layoutObjs);
  RGBColor backGroundColor;
  Camera* cameraPtr_;
  ViewPlane* viewPlanePtr_;
  Sampler2D* samplerPtr_;
  std::vector<Light*> lights_;
  LayoutType type_;
  std::unique_ptr<GeometricLayout> geometricLayoutPtr_;
};