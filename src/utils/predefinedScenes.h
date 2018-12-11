#include <iostream>
#include <time.h>

#include "../thirdParty/png.h"
#include "../thirdParty/rgbapixel.h"

#include "../Ray.h"
#include "../World.h"
#include "../geometricObjects/Plane.h"
#include "../geometricObjects/Sphere.h"
#include "../geometricObjects/Triangle.h"
#include "../materials/Material.h"
#include "../materials/Checker3D.h"
#include "../materials/fBmNoiseTexture.h"
#include "../samplers/MultiJitteredSampler2D.h"
#include "../samplers/Sampler2D.h"
#include "io/loadMeshes.h"
#include "RGBColor.h"
#include "Vec3.h"

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

void MP3(const std::string& filename)
{
  /* Initialize world */
  std::cout << "Initializing world setting" << std::endl;
  World world = World();

  Vec3 cp(0,0, 2);
  Vec3 la(0,0,-1);
  Vec3 up(0,1,0);

  PerspectiveCamera cameraP(cp, la, up, 2);

  Sampler2D sampler = Sampler2D();
  ViewPlane viewPlane(0.1,0.1,108,96);

  std::cout << "Applying world setting" << std::endl;
  world.SetGeometricLayoutType(LayoutType::GRID);
  world.SetViewPlane(&viewPlane);
  world.SetSampler(&sampler);

  /* Initializing materials */
  Material green = Material(RGBColor(0.0,1.0,0.0));
  Material redPurple = Material(RGBColor(1.0,0.0,0.5));
  Material lightRedPurple = Material(RGBColor(1.0,0.5,1.0));
  Material white = Material(RGBColor(1.0, 1.0, 1.0));

  Material whiteLightSource = Material();
  whiteLightSource.SimpleLightSource(RGBColor(1.0, 1.0, 1.0));

  Material transparentMaterial = Material();
  transparentMaterial.SimpleTransparentMaterial();

  Material mirror = Material();
  mirror.SimpleMirror();

  std::cout << "Creating geometric objects" << std::endl;
  SphereAreaLight sphere1(Vec3(-1, 1, 1), 0.5, &whiteLightSource, 64);

  Sphere sphere2(Vec3(-1.5, -1.5, 0.0), 1.0, &transparentMaterial);
  Sphere sph3(Vec3(-3.5, -2.0, -1.0), 1.0, &green);

  Triangle triangle2(Vec3(2, 0, 0), Vec3(2, -3, -1), Vec3(0, -2, 1),
    &redPurple);
  Triangle triangle2_(Vec3(0, -2, 1), Vec3(2, -3, -1), Vec3(2, 0, 0),
    &redPurple);

  Triangle squareMirrorPart1(Vec3(-2, -2, -1), Vec3(2, -2, -1), Vec3(2, 2, -1),
    &mirror);
  Triangle squareMirrorPart2(Vec3(2, 2, -1), Vec3(-2, 2, -1), Vec3(-2, -2, -1),
    &mirror);
  Triangle squareMirrorPart1_(Vec3(2, 2, -1), Vec3(2, -2, -1), Vec3(-2, -2, -1),
    &white);
  Triangle squareMirrorPart2_(Vec3(-2, -2, -1), Vec3(-2, 2, -1), Vec3(2, 2, -1),
    &white);

  Plane plane(Vec3(0.0, -3.0, 0.0), Vec3(0.0, 1.0, 0.0), &white);
  Plane plane2(Vec3(0.0, 3.0, 0.0), Vec3(0.0, -1.0, 0.0), &redPurple);
  Plane plane3(Vec3(0.0, 0.0, -1.5), Vec3(0.0, 0.0, 1.0), &white);
  Plane plane4(Vec3(0.0, 0.0, 2.5), Vec3(0.0, 0.0, -1.0), &lightRedPurple);
  Plane plane5(Vec3(-3.0, 0.0, 0.0), Vec3(1.0, 0.0, 0.0), &white);
  Plane plane6(Vec3(3.0, 0.0, 0.0), Vec3(-1.0, 0.0, 0.0), &white);

  std::cout << "Adding geometric objects" << std::endl;
  world.AddGeometricObject(&triangle2);
  world.AddGeometricObject(&triangle2_);
  world.AddGeometricObject(&sphere1);
  world.AddGeometricObject(&sphere2);
  world.AddGeometricObject(&sph3);

  world.AddGeometricObject(&squareMirrorPart1);
  world.AddGeometricObject(&squareMirrorPart2);
  world.AddGeometricObject(&squareMirrorPart1_);
  world.AddGeometricObject(&squareMirrorPart2_);

  world.AddGeometricObject(&plane);
  world.AddGeometricObject(&plane2);
  world.AddGeometricObject(&plane3);
  world.AddGeometricObject(&plane4);

  world.AddLightSource(&sphere1);

  time_t start;
  double seconds;
  Scene res;

  std::cout << "Rendering perspective scene" << std::endl;
  world.SetCamera(&cameraP);
  start = time(0);
  res = world.Render();
  ToPNG(filename, res);
  seconds = difftime(time(0), start);
  std::cout << "Rendered perspective in " << seconds << " seconds" << std::endl;
}

void MP4(const std::string& filename)
{
  /* Initialize world */
  std::cout << "Initializing world setting" << std::endl;
  World world = World();

  Vec3 cp(0,0, 10);
  Vec3 la(0,0,-1);
  Vec3 up(0,1,0);

  PerspectiveCamera cameraP(cp, la, up, 10);
  // OrthographicCamera cameraP(cp, la, up);

  Sampler2D sampler = Sampler2D();
  ViewPlane viewPlane(0.01,0.01,1080,960);

  std::cout << "Applying world setting" << std::endl;
  world.SetGeometricLayoutType(LayoutType::GRID);
  world.SetViewPlane(&viewPlane);
  world.SetSampler(&sampler);

  /* Initializing materials */
  Material green = Material(RGBColor(0.0,1.0,0.0));
  Material redPurple = Material(RGBColor(1.0,0.0,0.5));
  Material lightRedPurple = Material(RGBColor(1.0,0.5,1.0));
  Material white = Material(RGBColor(1.0, 1.0, 1.0));

  Material whiteLightSource = Material();
  whiteLightSource.SimpleLightSource(RGBColor(1.0, 1.0, 1.0));

  Checker3D checker = Checker3D(RGBColor(1.0, 1.0, 1.0));
  fBmNoiseTexture noise = fBmNoiseTexture(RGBColor(0.0, 1.0, 0.0), 0.7);

  std::cout << "Creating geometric objects" << std::endl;
  //SphereAreaLight sphere1(Vec3(0, 0, 3.0), 0.5, &whiteLightSource, 64);
  PointLight light(Vec3(0.0, 0.0, 3.0), RGBColor(1.0, 1.0, 1.0));

  Sphere sphere2(Vec3(-1.5, -1.5, 0.0), 1.0, &checker);
  Sphere sphere3(Vec3(1.5, 2.0, -1.0), 1.0, &noise);

  Plane plane(Vec3(0.0, -3.0, 0.0), Vec3(0.0, 1.0, 0.0), &white);
  Plane plane3(Vec3(0.0, 0.0, -2.5), Vec3(0.0, 0.0, 1.0), &white);

  std::cout << "Adding geometric objects" << std::endl;
  //world.AddGeometricObject(&sphere1);
  world.AddGeometricObject(&sphere2);
  world.AddGeometricObject(&sphere3);

  world.AddGeometricObject(&plane);
  world.AddGeometricObject(&plane3);

  //world.AddLightSource(&sphere1);
  world.AddLightSource(&light);

  time_t start;
  double seconds;
  Scene res;

  std::cout << "Rendering perspective scene" << std::endl;
  world.SetCamera(&cameraP);
  start = time(0);
  res = world.Render();
  ToPNG(filename, res);
  seconds = difftime(time(0), start);
  std::cout << "Rendered perspective in " << seconds << " seconds" << std::endl;
}