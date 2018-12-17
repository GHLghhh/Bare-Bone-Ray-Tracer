#include <cmath>
#include <iostream>
#include <thread>
#include <future>

#include "World.h"
#include "shaders/Shader.h"
#include "utils/constants.h"

static double WORLD_INDEX_OF_REFREACTION = 1.0;

World::World()
  : cameraPtr_(nullptr), viewPlanePtr_(nullptr), samplerPtr_(nullptr),
    diffuseDirectionSampler_(HemisphereSampler3D(10000)),
    oneDiffuseDirectionSampler_(HemisphereSampler3D(1)),
    irradianceCache_(IrradianceCache()), type_(LayoutType::LIST)
{
  geometricLayoutPtr_ = std::unique_ptr<GeometricLayout>(new GeometricLayout());
}

World::World(const World& rhs)
  : backGroundColor(rhs.backGroundColor),
    cameraPtr_(rhs.cameraPtr_), viewPlanePtr_(rhs.viewPlanePtr_),
    samplerPtr_(rhs.samplerPtr_), lights_(rhs.lights_),
    diffuseDirectionSampler_(HemisphereSampler3D(10000)),
    oneDiffuseDirectionSampler_(HemisphereSampler3D(1)),
    irradianceCache_(IrradianceCache()), type_(rhs.type_)
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

Scene World::Render(const int recursionDepth)
{
  Scene res;
  if (cameraPtr_ == nullptr || viewPlanePtr_ == nullptr || samplerPtr_ == nullptr) {
    // [TODO] error handling
    std::cout << "World not set up properly" << std::endl;
    return res;
  }
  PixelRays pixelRays = cameraPtr_->CastRays(*samplerPtr_, *viewPlanePtr_);
  size_t totalPixel = pixelRays.size();
  std::cout << "First pass" << std::endl;
  int counter = 0;
  for (auto& pixelsInRow : pixelRays) {
    // res.push_back(std::vector<RGBColor>());
    for (auto& pixelsInRowColumn : pixelsInRow) {
        // Get proper color for each sample for the pixel and take average
        RGBColor resColor(0.0, 0.0, 0.0);
        for (Ray& ray : pixelsInRowColumn) {
          resColor += TraceRay(ray, 0, recursionDepth).first;
        }
        // res.back().push_back(resColor / pixelsInRowColumn.size());
    }
    counter++;
    if (counter % (std::max((int)(totalPixel / 10),1)) == 0) {
      std::cout << "Rendering current scene: " << counter << "/" << totalPixel << std::endl;
    }
  }
  std::cout << "Second pass" << std::endl;
  for (auto& pixelsInRow : pixelRays) {
    res.push_back(std::vector<RGBColor>());
    for (auto& pixelsInRowColumn : pixelsInRow) {
        // Get proper color for each sample for the pixel and take average
        RGBColor resColor(0.0, 0.0, 0.0);
        for (Ray& ray : pixelsInRowColumn) {
          resColor += TraceRay(ray, 0, recursionDepth).first;
        }
        res.back().push_back(resColor / pixelsInRowColumn.size());
    }
    if (res.size() % (std::max((int)(totalPixel / 10),1)) == 0) {
      std::cout << "Rendering current scene: " << res.size() << "/" << totalPixel << std::endl;
    }
  }
  return res;
}

std::pair<RGBColor, double> World::TraceRay(const Ray& ray, const int currentDepth, const int recursionDepth)
{
  RGBColor res(0.0, 0.0, 0.0);
  double t = -1;
  ShadeRec sr(ray.Position());
  bool isHit = geometricLayoutPtr_->Hit(ray, t, sr);
  // No hit, use background color for this sample
  if (!isHit) {
    res += backGroundColor;
  } else {
    RGBColor tempRes = RGBColor(0.0, 0.0, 0.0);
    // emittedColor is 0 if it is not hitting light source
    if (currentDepth != 1)
      res += sr.material->emittedColor;
    // [TODO] ambient component and wrap everything into a shading model?
    // Direct illumination
    if (currentDepth == 0)
      res += DirectIllumination(sr);
    // [TODO] right now, indirect illumination will only happen on Mirror
    // (specular 1.0, diffuse 0.0)
    // int numSample = (currentDepth == 0) ? 100 : 1;
    
    // for (int i = 0; i < numSample; i++)
    //   tempRes += IndirectIllumination(sr, currentDepth, recursionDepth);
    // res += tempRes / numSample;
    res += IndirectIllumination(sr, currentDepth, recursionDepth);
  }
  return std::make_pair(res, t);
}

RGBColor World::DirectIllumination(const ShadeRec& sr)
{
  RGBColor res(0.0, 0.0, 0.0);
  for (Light* light : lights_) {
    AreaLight* areaLightCast = dynamic_cast<AreaLight*>(light);

    RGBColor unweightedResColor(0.0, 0.0, 0.0);
    double tTemp = -1;
    std::vector<ToLightRecord> toLightRecords = 
      light->ToLightRecords(sr.hitPosition);
    // std::cout << "Hit position: " << sr.hitPosition << std::endl;
    
    for (ToLightRecord& toLightRecord : toLightRecords) {
      ShadeRec srTemp(sr.eyePosition);
      // std::cout << srTemp.objectHit << std::endl;
      // toLightDirection for area light is instantiated differently
      Vec3 toLightDirection = (areaLightCast) ? (toLightRecord.first - sr.hitPosition).Unit() : toLightRecord.first;
      // std::cout << toLightDirection << " : " << toLightRecord.second << std::endl;
      Ray shadowRay(sr.hitPosition, toLightDirection);

      // if (areaLightCast) {
      //   std::cout << "Area light: " << light << std::endl;
      // }
      bool isBlocked = geometricLayoutPtr_->Hit(
        shadowRay, tTemp, srTemp, true, toLightRecord.second);

      // Object hit matches current light only happens in area light
      // [TODO] if block by other area light, will have shadow. Check why it happens
      if (isBlocked && (srTemp.objectHit == light || srTemp.material->transmissiveCoefficient > 0)) {
        isBlocked = false;
      }
      if (!isBlocked) {
        RGBColor tempRes(0.0, 0.0, 0.0);
        if (areaLightCast) {
          // [TODO] Area form of rendering equation
          // [TODO] make diffuse and specular BRDF
          tempRes += 
            Shader::Diffuse(sr, toLightDirection, *light)
            * Vec3::Dot(areaLightCast->Normal(toLightRecord.first), toLightDirection * -1)
            * Vec3::Dot(toLightDirection, sr.normal)
            // [TODO] figure out how to model it correctly.
            / pow((sr.hitPosition - toLightRecord.first).Length() / 4, 2)
            * areaLightCast->InvPDF(toLightRecord.first);
        } else {
          // [TODO] change it into hemisphere form
          tempRes += Shader::Diffuse(sr, toLightDirection, *light);
          tempRes += Shader::Specular(sr, toLightDirection, *light);
        }
        unweightedResColor += tempRes * ((srTemp.material != nullptr && srTemp.material->transmissiveCoefficient > 0) ? srTemp.material->transmissiveCoefficient : 1.0);
      }
    }
    res += (unweightedResColor / toLightRecords.size());
  }
  return res;
}

std::tuple<RGBColor, double, double> World::PartialRender(std::vector<Vec3>* localDirection_ptr, Vec3 u, Vec3 v, Vec3 w, Vec3 hitPosition, int currentDepth, int recursionDepth, int start, int end)
{
  double invDistanceSum = 0;
  double numHit = 0.0;
  RGBColor res = RGBColor(0.0, 0.0, 0.0);
  for (int i = start; i < end; i++) {
    Vec3 diffusedDirection = (u * (*localDirection_ptr)[i].x + v * (*localDirection_ptr)[i].y + w * (*localDirection_ptr)[i].z).Unit();
    auto traceRes = TraceRay(Ray(hitPosition, diffusedDirection), currentDepth + 1, recursionDepth);
    res += traceRes.first;
    if (traceRes.second > 0) {
        invDistanceSum += 1.0 / traceRes.second;
        numHit += 1.0;
    }
  }
  return std::make_tuple(res, invDistanceSum, numHit);
}

RGBColor World::IndirectIllumination(const ShadeRec& sr, const int currentDepth, const int recursionDepth, int inverseNormal)
{
  // End indirect illumination
  if (currentDepth >= recursionDepth) {
    return RGBColor(0.0, 0.0, 0.0);
  }

  RGBColor res(0.0, 0.0, 0.0);
  RGBColor incomingColor(0.0, 0.0, 0.0);
  if (sr.material->diffuseCoefficient > 0) {
    // Diffuse
    // [TODO] extract part of it out once BRDF is implemented as separate components
    // Create u, v, w unit vector for mapping hemisphere samples on world coordinate
    Vec3 w = sr.normal;
    // Variable used for generating orthogonal coordinate
    // Vec3 driftW = Vec3(
    //   Vec3::Dot(Vec3(cos(M_PI_4), -sin(M_PI_4), 0.0), w),
    //   Vec3::Dot(Vec3(sin(M_PI_4), cos(M_PI_4), 0.0), w),
    //   Vec3::Dot(Vec3(0.0, 0.0, 1.0), w));

    Vec3 u = Vec3::Cross(w, (fabs(w.x)>.1?Vec3(0.0,1.0,0.0):Vec3(1.0, 0.0, 0.0))).Unit();
    Vec3 v = Vec3::Cross(w, u).Unit();

    std::vector<Vec3> localDiffusedDirection;
    if (currentDepth == 0) {
      // Only do irradiance caching for primary ray
      IrradianceCache::ResultIrradianceQueue iqueue;
      const double threashold = 5.0; // 大きいほど精度向上
      const double radius = 100.0;
      IrradianceCache::IrradianceQuery query(sr.hitPosition, sr.normal, radius, threashold);
      irradianceCache_.SearchCachedPoints(iqueue, query);
      if (iqueue.size() == 0)
        localDiffusedDirection = diffuseDirectionSampler_.GenerateSamplePoints();
      else {
        // get incoming color from cache
        std::vector<IrradianceCache::ElementForIrradianceQueue> irrs;
        irrs.reserve(iqueue.size());
        double weight = 0;
        for (;!iqueue.empty();) {
          IrradianceCache::ElementForIrradianceQueue i = iqueue.top();
          iqueue.pop();
          irrs.push_back(i);
          // [TODO] special case for exact same point
          if (i.weight == -1) {
            incomingColor = i.point->irradiance;
            weight = 1;
            break;
          }
          weight += i.weight;
          incomingColor += i.point->irradiance * i.weight;
        }

        incomingColor = incomingColor / weight;
      }
    } else {
      localDiffusedDirection = oneDiffuseDirectionSampler_.GenerateSamplePoints();
    }
    double invDistanceSum = 0;
    // double for calculation
    double numHit = 0;
    // handle tracing on direction size > 1
    if (localDiffusedDirection.size() > 1) {
      int deciSize = localDiffusedDirection.size() / 40;
      std::vector<std::future<std::tuple<RGBColor, double, double>>> incomingRes;
      for (int i = 0; i < localDiffusedDirection.size(); i+=deciSize) {
        int end = (i+deciSize > localDiffusedDirection.size()) ? localDiffusedDirection.size() : (i+deciSize);
        incomingRes.push_back(std::async(&World::PartialRender, this, &localDiffusedDirection, u, v, w, sr.hitPosition, currentDepth, recursionDepth, i, end));
      }
      for (auto& oneRes : incomingRes) {
        auto resTuple = oneRes.get();
        incomingColor += std::get<0>(resTuple);
        invDistanceSum += std::get<1>(resTuple);
        numHit += std::get<2>(resTuple);
      }
      // If here, cache miss, add sampling result to cache
      incomingColor = incomingColor / localDiffusedDirection.size();
      irradianceCache_.AddPointToTree(Irradiance(sr.hitPosition, incomingColor, sr.normal, numHit / invDistanceSum));
    // handle tracing on direction size = 1
    } else if (localDiffusedDirection.size() == 1) {
      Vec3 diffusedDirection = (u * localDiffusedDirection[0].x + v * localDiffusedDirection[0].y + w * localDiffusedDirection[0].z).Unit();
      incomingColor += TraceRay(Ray(sr.hitPosition, diffusedDirection), currentDepth + 1, recursionDepth).first;
    }
    // [TODO] Directly using pdf for diffuse surface and simplify the equation with it.
    // Need to make it into getting actual pdf to generalize on other types of surfaces
    res += incomingColor * sr.material->color * sr.material->diffuseCoefficient * M_PI_2;
  }

  // [TODO] Right now is only perfect mirror reflection
  // Reflection
  // Get reflected ray and trace
  Vec3 toEyeDirection = (sr.eyePosition - sr.hitPosition).Unit();
  if (sr.material->specularCoefficient > 0) {
    Vec3 reflectedDirection = sr.normal * Vec3::Dot(sr.normal * inverseNormal, toEyeDirection) * 2 * inverseNormal - toEyeDirection;
    
    if (inverseNormal == 1) {
      incomingColor = TraceRay(Ray(sr.hitPosition, reflectedDirection), currentDepth+1, recursionDepth).first;
    } else {
      incomingColor = TraceRayInObject(Ray(sr.hitPosition, reflectedDirection), currentDepth+1, recursionDepth);
    }
    res += incomingColor * sr.material->specularCoefficient * sr.material->color;
  }

  // Refraction
  // [TODO] reuse TraceRay
  if (sr.material->transmissiveCoefficient > 0) {
    double refractionRatio = (inverseNormal != 1) ?
      (WORLD_INDEX_OF_REFREACTION / sr.material->indexOfRefraction) : (sr.material->indexOfRefraction / WORLD_INDEX_OF_REFREACTION);
    double cosThetaI = Vec3::Dot(sr.normal * inverseNormal, toEyeDirection);
    // Also use to check Total Internal Reflection
    double cosThetaTSquare = (1.0 - (1.0 - cosThetaI * cosThetaI) / (refractionRatio * refractionRatio));
    if (cosThetaTSquare >= 0) {
      double cosThetaT = pow(cosThetaTSquare, 0.5);
      Vec3 transmissiveDirection = toEyeDirection * -1.0 / refractionRatio
        - sr.normal * inverseNormal * (cosThetaT - cosThetaI / refractionRatio);
      // From outside of object to inside of object
      if (inverseNormal == 1) {
        incomingColor = TraceRayInObject(Ray(sr.hitPosition, transmissiveDirection), currentDepth+1, recursionDepth);
      } else {
        incomingColor = TraceRay(Ray(sr.hitPosition, transmissiveDirection), currentDepth+1, recursionDepth).first;
      }
      res += incomingColor * sr.material->transmissiveCoefficient * 1.0 / (refractionRatio * refractionRatio);
    } else {
      res += incomingColor * (1.0 - sr.material->specularCoefficient) * sr.material->color;
    }
  }

  return res;
}

RGBColor World::TraceRayInObject(const Ray& ray, const int currentDepth, const int recursionDepth)
{
  RGBColor res(0.0, 0.0, 0.0);
  double t = -1;
  ShadeRec sr(ray.Position());
  bool isHit = geometricLayoutPtr_->Hit(ray, t, sr, false, -1.0, true);
  // No hit, use background color for this sample
  if (!isHit) {
    res += backGroundColor;
  } else {
    //std::cout << "Here" << std::endl;
    res += IndirectIllumination(sr, currentDepth, recursionDepth, -1);
  }
  return res;
}