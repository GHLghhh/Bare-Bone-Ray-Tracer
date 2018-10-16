#pragma once

#include <vector>
#include "Camera.h"
#include "Light.h"
#include "samplers/Sampler2D.h"
#include "geometricObjects/GeometricObject.h"
#include "GeometricLayout.h"

using Scene = std::vector<std::vector<RGBColor>>;

class World {
public:
  World();
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
  RGBColor backGroundColor;
  Camera* cameraPtr_;
  ViewPlane* viewPlanePtr_;
  Sampler2D* samplerPtr_;
  std::vector<Light*> lights_;
  GeometricLayout* geometricLayoutPtr_;
};