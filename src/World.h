#pragma once

#include <memory>
#include <vector>

#include "Camera.h"
#include "GeometricLayout.h"
#include "Light.h"
#include "acceleratingStructures/IrradianceCache.h"
#include "geometricObjects/GeometricObject.h"
#include "samplers/Sampler2D.h"
#include "samplers/HemisphereSampler3D.h"
#include "utils/ShadeRec.h"

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

  Scene Render(const int recursionDepth = 0);

  std::vector<Vec3*> GetCachePoints() {
    return irradianceCache_.GetRawVector();
  }
private:
  void ConvertFromExistingLayout(
    LayoutType type, std::vector<GeometricObject*> layoutObjs);

  std::pair<RGBColor, double> TraceRay(const Ray& ray, const int currentDepth, const int recursionDepth = 0);
  RGBColor TraceRayInObject(const Ray& ray, const int currentDepth, const int recursionDepth = 0);
  RGBColor DirectIllumination(const ShadeRec& sr);
  RGBColor IndirectIllumination(const ShadeRec& sr, const int currentDepth, const int recursionDepth, int inverseNormal = 1);

  RGBColor backGroundColor;
  Camera* cameraPtr_;
  ViewPlane* viewPlanePtr_;
  Sampler2D* samplerPtr_;
  HemisphereSampler3D diffuseDirectionSampler_;
  HemisphereSampler3D oneDiffuseDirectionSampler_;
  IrradianceCache irradianceCache_;
  std::vector<Light*> lights_;
  LayoutType type_;
  std::unique_ptr<GeometricLayout> geometricLayoutPtr_;
};