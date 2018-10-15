#include <iostream>
#include "Ray.h"
#include "geometricObjects/Plane.h"
#include "geometricObjects/Triangle.h"
#include "geometricObjects/Sphere.h"
#include "utils/Vec3.h"
#include "utils/RGBColor.h"
#include "utils/ShadeRec.h"
#include "World.h"
#include "samplers/MultiJitteredSampler2D.h"

#include "external/png.h"
#include "external/rgbapixel.h"

void ToPNG(const std::string& filename, const Scene& scene)
{
  // [TODO] input checking
  int width = scene.size();
  int height = scene[0].size();
  PNG out(width, height);
  // PNG lib use (0,0) as upper-left corner, y++ goes down and x++ goes right
  // Ray tracer use (0,0) as bottom-left corner, y++ goes up and x++ goes right
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      RGBColor pixel = scene[x][y].MaxToOne().To8Bit();
      *out(x, (height-1)-y) = RGBAPixel(pixel.x, pixel.y, pixel.z);
    }
  }
  out.writeToFile(filename);
}

int main() {
  World world = World();

  Vec3 cp = Vec3(0,0,2);
  Vec3 la = Vec3(0,0,-1);
  Vec3 up = Vec3(0,1,0);
  PerspectiveCamera cameraP(cp, la, up, 2);
  OrthographicCamera cameraO(cp, la, up);
  PerspectiveCamera cameraP2(Vec3(3,0,0), Vec3(-1, 0, 0), up, 2);

  ViewPlane viewPlane(0.05,0.05,100,100);

  Sampler2D sampler = Sampler2D();
  MultiJitteredSampler2D samplerM = MultiJitteredSampler2D(4, 4);

  PointLight light(Vec3(5, 10, 1), Vec3(0.5, 1.0, 1.0));
  PointLight light2(Vec3(10, 0, 1), Vec3(1.0, 0.5, 0.5));
  Triangle triangle1(Vec3(-1, -1, 0), Vec3(1, -1, 0), Vec3(1, 1, 0),
    RGBColor(0.0,1.0,0.0));
  Triangle triangle2(Vec3(0, -2, 1), Vec3(2, -3, -1), Vec3(2, 0, 0),
    RGBColor(1.0,0.0,0.5));

  Sphere sphere1(Vec3(-1, 0.75, 0), 1.0, RGBColor(1.0,1.0,1.0));
  Sphere sphere2(Vec3(-1, -0.75, 0), 1.0, RGBColor(1.0,0.5,1.0));

  world.SetViewPlane(&viewPlane);
  world.AddLightSource(&light);
  world.AddLightSource(&light2);
  world.AddGeometricObject(&triangle1);
  world.AddGeometricObject(&triangle2);
  world.AddGeometricObject(&sphere1);
  world.AddGeometricObject(&sphere2);

  world.SetCamera(&cameraP);
  world.SetSampler(&sampler);

  Scene res = world.Render();
  ToPNG("outs/singleRay.png", res);

  world.SetSampler(&samplerM);
  res = world.Render();
  ToPNG("outs/multiJittering.png", res);

  // [TODO] check if orthographic camera is shaded correctly.
  world.SetCamera(&cameraO);
  res = world.Render();
  ToPNG("outs/orthographic.png", res);

  world.SetCamera(&cameraP2);
  res = world.Render();
  ToPNG("outs/perspective2.png", res);

  return 0;
}
