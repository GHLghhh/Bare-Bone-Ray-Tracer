#include <iostream>
#include <time.h>

#include "../thirdParty/png.h"
#include "../thirdParty/rgbapixel.h"

#include "../Ray.h"
#include "../World.h"
#include "../geometricObjects/Plane.h"
#include "../geometricObjects/Sphere.h"
#include "../geometricObjects/Triangle.h"
#include "../geometricObjects/CSGObject.h"
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

  // Sampler2D sampler = Sampler2D();
  MultiJitteredSampler2D sampler = MultiJitteredSampler2D(4, 4);
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

  Sphere lSph(Vec3(-1.0, 0.5, -0.5), 1.0, &green);
  Sphere rSph(Vec3(0.0, 0.5, -0.5), 1.0, &green);
  CSGObject csgObject(&lSph, &rSph, CSGOperation::INTERSECTION);

  Sphere sphere2(Vec3(-1.5, -1.5, 0.0), 1.0, &checker);
  Sphere sphere3(Vec3(1.5, 2.0, -1.0), 1.0, &noise);

  Plane plane(Vec3(0.0, -3.0, 0.0), Vec3(0.0, 1.0, 0.0), &white);
  Plane plane3(Vec3(0.0, 0.0, -2.5), Vec3(0.0, 0.0, 1.0), &white);

  std::cout << "Adding geometric objects" << std::endl;
  //world.AddGeometricObject(&sphere1);
  world.AddGeometricObject(&sphere2);
  world.AddGeometricObject(&sphere3);
  world.AddGeometricObject(&csgObject);

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

void VideoSceneA(const std::string& filename)
{
  /* Initialize world */
  std::cout << "Initializing world setting" << std::endl;
  World world = World();

  Vec3 cp(0,0, 10);
  Vec3 la(0,0,-1);
  Vec3 up(0,1,0);

  PerspectiveCamera cameraP(cp, la, up, 10);

  MultiJitteredSampler2D sampler = MultiJitteredSampler2D(2, 2);
  ViewPlane viewPlane(0.05,0.05,216,192);

  std::cout << "Applying world setting" << std::endl;
  world.SetGeometricLayoutType(LayoutType::LIST);
  world.SetViewPlane(&viewPlane);
  world.SetSampler(&sampler);

  /* Initializing materials */
  Material green = Material(RGBColor(0.0,1.0,0.0));
  Material redPurple = Material(RGBColor(1.0,0.0,0.5));
  Material yellow = Material(RGBColor(0.5,1.0,1.0));
  Material white = Material(RGBColor(1.0, 1.0, 1.0));
  Material black = Material(RGBColor(0.5, 0.5, 0.5));

  Material whiteLightSource = Material();
  whiteLightSource.SimpleLightSource(RGBColor(1.0, 1.0, 1.0));

  Material transparentMaterial = Material();
  transparentMaterial.SimpleTransparentMaterial();

  std::cout << "Creating geometric objects" << std::endl;

  PointLight light(Vec3(0.0, 1.0, 2.0), RGBColor(1.0, 1.0, 1.0));

  Sphere sphere2(Vec3(-1.5, -2.0, 0.0), 1.0, &redPurple);
  Sphere sphere3(Vec3(1.5, -2.0, -1.0), 1.0, &green);
  Sphere movingSphere(Vec3(-2.5, 2.0, 5.0), 0.5, &white);


  Plane plane(Vec3(0.0, -3.0, 0.0), Vec3(0.0, 1.0, 0.0), &white);
  Plane plane2(Vec3(0.0, 3.0, 0.0), Vec3(0.0, -1.0, 0.0), &redPurple);
  Plane plane3(Vec3(0.0, 0.0, -2.0), Vec3(0.0, 0.0, 1.0), &black);
  Plane plane5(Vec3(-3.0, 0.0, 0.0), Vec3(1.0, 0.0, 0.0), &yellow);
  Plane plane6(Vec3(3.0, 0.0, 0.0), Vec3(-1.0, 0.0, 0.0), &yellow);

  std::cout << "Adding geometric objects" << std::endl;
  world.AddGeometricObject(&sphere2);
  world.AddGeometricObject(&sphere3);
  world.AddGeometricObject(&movingSphere);

  world.AddGeometricObject(&plane);
  world.AddGeometricObject(&plane3);
  world.AddGeometricObject(&plane5);
  world.AddGeometricObject(&plane6);

  world.AddLightSource(&light);

  time_t start;
  double seconds;
  Scene res;

  std::cout << "Rendering perspective scene" << std::endl;
  world.SetCamera(&cameraP);
  start = time(0);
  Vec3 position = movingSphere.Position();
  double yShift = 0.0;
  double gravity = -0.01;
  for (int i = 0; i < 48; i++) {
    res = world.Render(0);
    if (i < 32) {
      position.x += 0.15625;
    } else {
      position.x -= 0.15625;
    }
    position.z -= 0.0625;
    if (i == 30) {
      yShift *= -1;
    }
    yShift += gravity;
    position.y += yShift;
    movingSphere.SetPosition(position.x, position.y, position.z);
    ToPNG(filename + std::to_string(i) + ".png", res);
  }
  for (int i = 0; i < 4; i++) {
    res = world.Render(0);
    ToPNG(filename + std::to_string(48 + i) + ".png", res);
  }
  seconds = difftime(time(0), start);
  std::cout << "Rendered perspective in " << seconds << " seconds" << std::endl;
}

void VideoSceneB(const std::string& filename)
{
  /* Initialize world */
  std::cout << "Initializing world setting" << std::endl;
  World world = World();

  Vec3 cp(0,0, 10);
  Vec3 la(0,0,-1);
  Vec3 up(0,1,0);

  PerspectiveCamera cameraP(cp, la, up, 10);

  MultiJitteredSampler2D sampler = MultiJitteredSampler2D(2, 2);
  ViewPlane viewPlane(0.05,0.05,216,192);

  std::cout << "Applying world setting" << std::endl;
  world.SetGeometricLayoutType(LayoutType::LIST);
  world.SetViewPlane(&viewPlane);
  world.SetSampler(&sampler);

  /* Initializing materials */
  Material green = Material(RGBColor(0.0,1.0,0.0));
  Material redPurple = Material(RGBColor(1.0,0.0,0.5));
  Material yellow = Material(RGBColor(0.5,1.0,1.0));
  Material white = Material(RGBColor(1.0, 1.0, 1.0));
  Material black = Material(RGBColor(0.5, 0.5, 0.5));

  Material whiteLightSource = Material();
  whiteLightSource.SimpleLightSource(RGBColor(1.0, 1.0, 1.0));

  Material transparentMaterial = Material();
  transparentMaterial.SimpleTransparentMaterial();

  std::cout << "Creating geometric objects" << std::endl;
  SphereAreaLight lightSphere(Vec3(0.0, 1.0, 2.0), 0.5, &whiteLightSource, 8);
  whiteLightSource.emittedColor = RGBColor(0.05, 0.05, 0.05);
  lightSphere.SetColor(RGBColor(0.05, 0.05, 0.05));

  Sphere sphere2(Vec3(-1.5, -2.0, 0.0), 1.0, &redPurple);
  Sphere sphere3(Vec3(1.5, -2.0, -1.0), 1.0, &green);
  Sphere movingSphere(Vec3(0.0, 1.0, 2.0), 0.5, &transparentMaterial);


  Plane plane(Vec3(0.0, -3.0, 0.0), Vec3(0.0, 1.0, 0.0), &white);
  Plane plane2(Vec3(0.0, 3.0, 0.0), Vec3(0.0, -1.0, 0.0), &redPurple);
  Plane plane3(Vec3(0.0, 0.0, -2.0), Vec3(0.0, 0.0, 1.0), &black);
  // Plane plane4(Vec3(0.0, 0.0, 2.5), Vec3(0.0, 0.0, -1.0), &lightRedPurple);
  Plane plane5(Vec3(-3.0, 0.0, 0.0), Vec3(1.0, 0.0, 0.0), &yellow);
  Plane plane6(Vec3(3.0, 0.0, 0.0), Vec3(-1.0, 0.0, 0.0), &yellow);

  std::cout << "Adding geometric objects" << std::endl;
  world.AddGeometricObject(&lightSphere);
  world.AddGeometricObject(&sphere2);
  world.AddGeometricObject(&sphere3);

  world.AddGeometricObject(&plane);
  world.AddGeometricObject(&plane3);
  world.AddGeometricObject(&plane5);
  world.AddGeometricObject(&plane6);

  world.AddLightSource(&lightSphere);

  time_t start;
  double seconds;
  Scene res;

  std::cout << "Rendering perspective scene" << std::endl;
  world.SetCamera(&cameraP);
  start = time(0);
  double cv = 0.05;
  for (int i = 0; i < 48; i++) {
    cv += 0.0125;
    whiteLightSource.emittedColor = RGBColor(cv, cv, cv);
    lightSphere.SetColor(RGBColor(cv, cv, cv));
    res = world.Render(0);
    ToPNG(filename + std::to_string(i) + ".png", res);
  }
  for (int i = 0; i < 4; i++) {
    res = world.Render(0);
    ToPNG(filename + std::to_string(48 + i) + ".png", res);
  }
  seconds = difftime(time(0), start);
  std::cout << "Rendered perspective in " << seconds << " seconds" << std::endl;
}

void VideoSceneC(const std::string& filename)
{
  /* Initialize world */
  std::cout << "Initializing world setting" << std::endl;
  World world = World();

  Vec3 cp(0,0, 10);
  Vec3 la(0,0,-1);
  Vec3 up(0,1,0);

  PerspectiveCamera cameraP(cp, la, up, 10);

  MultiJitteredSampler2D sampler = MultiJitteredSampler2D(2, 2);
  ViewPlane viewPlane(0.05,0.05,216,192);

  std::cout << "Applying world setting" << std::endl;
  world.SetGeometricLayoutType(LayoutType::LIST);
  world.SetViewPlane(&viewPlane);
  world.SetSampler(&sampler);

  /* Initializing materials */
  Material green = Material(RGBColor(0.0,1.0,0.0));
  Material redPurple = Material(RGBColor(1.0,0.0,0.5));
  Material yellow = Material(RGBColor(0.5,1.0,1.0));
  Material white = Material(RGBColor(1.0, 1.0, 1.0));
  Material black = Material(RGBColor(0.5, 0.5, 0.5));

  Material whiteLightSource = Material();
  whiteLightSource.SimpleLightSource(RGBColor(1.0, 1.0, 1.0));

  Material transparentMaterial = Material();
  transparentMaterial.SimpleTransparentMaterial();

  std::cout << "Creating geometric objects" << std::endl;
  SphereAreaLight lightSphere(Vec3(0.0, 1.0, 2.0), 0.5, &whiteLightSource, 8);
  whiteLightSource.emittedColor = RGBColor(0.05, 0.05, 0.05);
  lightSphere.SetColor(RGBColor(0.05, 0.05, 0.05));

  Sphere sphere2(Vec3(-1.5, -2.0, 0.0), 1.0, &redPurple);
  Sphere sphere3(Vec3(1.5, -2.0, -1.0), 1.0, &green);
  Sphere movingSphere(Vec3(0.0, 1.0, 2.0), 0.5, &transparentMaterial);


  Plane plane(Vec3(0.0, -3.0, 0.0), Vec3(0.0, 1.0, 0.0), &white);
  Plane plane2(Vec3(0.0, 3.0, 0.0), Vec3(0.0, -1.0, 0.0), &redPurple);
  Plane plane3(Vec3(0.0, 0.0, -2.0), Vec3(0.0, 0.0, 1.0), &black);
  // Plane plane4(Vec3(0.0, 0.0, 2.5), Vec3(0.0, 0.0, -1.0), &lightRedPurple);
  Plane plane5(Vec3(-3.0, 0.0, 0.0), Vec3(1.0, 0.0, 0.0), &yellow);
  Plane plane6(Vec3(3.0, 0.0, 0.0), Vec3(-1.0, 0.0, 0.0), &yellow);

  std::cout << "Adding geometric objects" << std::endl;
  world.AddGeometricObject(&lightSphere);
  world.AddGeometricObject(&sphere2);
  world.AddGeometricObject(&sphere3);

  world.AddGeometricObject(&plane);
  world.AddGeometricObject(&plane3);
  world.AddGeometricObject(&plane5);
  world.AddGeometricObject(&plane6);

  world.AddLightSource(&lightSphere);

  time_t start;
  double seconds;
  Scene res;

  std::cout << "Rendering perspective scene" << std::endl;
  world.SetCamera(&cameraP);
  start = time(0);
  double cv = 0.65;
  double radius = 0.5;
  double radiusShift = -0.4 / 48;
  for (int i = 0; i < 48; i++) {
    cv += 0.01;
    radius += radiusShift;
    whiteLightSource.emittedColor = RGBColor(cv, cv, cv);
    lightSphere.SetColor(RGBColor(cv, cv, cv));
    lightSphere.SetRadius(radius);
    res = world.Render(0);
    ToPNG(filename + std::to_string(i) + ".png", res);
  }
  for (int i = 0; i < 4; i++) {
    res = world.Render(0);
    ToPNG(filename + std::to_string(48 + i) + ".png", res);
  }

  seconds = difftime(time(0), start);
  std::cout << "Rendered perspective in " << seconds << " seconds" << std::endl;
}

void VideoSceneD(const std::string& filename)
{
  /* Initialize world */
  std::cout << "Initializing world setting" << std::endl;
  World world = World();

  Vec3 cp(0,0, 10);
  Vec3 la(0,0,-1);
  Vec3 up(0,1,0);

  PerspectiveCamera cameraP(cp, la, up, 10);

  MultiJitteredSampler2D sampler = MultiJitteredSampler2D(2, 2);
  ViewPlane viewPlane(0.05,0.05,216,192);

  std::cout << "Applying world setting" << std::endl;
  world.SetGeometricLayoutType(LayoutType::LIST);
  world.SetViewPlane(&viewPlane);
  world.SetSampler(&sampler);

  /* Initializing materials */
  Material green = Material(RGBColor(0.0,1.0,0.0));
  Material redPurple = Material(RGBColor(1.0,0.0,0.5));
  Material yellow = Material(RGBColor(0.5,1.0,1.0));
  Material white = Material(RGBColor(1.0, 1.0, 1.0));
  Material black = Material(RGBColor(0.5, 0.5, 0.5));

  Material whiteLightSource = Material();
  whiteLightSource.SimpleLightSource(RGBColor(1.0, 1.0, 1.0));

  Material transparentMaterial = Material();
  transparentMaterial.SimpleTransparentMaterial();

  std::cout << "Creating geometric objects" << std::endl;
  SphereAreaLight lightSphere(Vec3(0.0, 1.0, 2.0), 0.1, &whiteLightSource, 8);

  Sphere sphere2(Vec3(-1.5, -2.0, 0.0), 1.0, &redPurple);
  Sphere sphere3(Vec3(1.5, -2.0, -1.0), 1.0, &green);
  Sphere movingSphere(Vec3(0.0, 1.0, 2.0), 0.5, &transparentMaterial);


  Plane plane(Vec3(0.0, -3.0, 0.0), Vec3(0.0, 1.0, 0.0), &white);
  Plane plane2(Vec3(0.0, 3.0, 0.0), Vec3(0.0, -1.0, 0.0), &redPurple);
  Plane plane3(Vec3(0.0, 0.0, -2.0), Vec3(0.0, 0.0, 1.0), &black);
  // Plane plane4(Vec3(0.0, 0.0, 2.5), Vec3(0.0, 0.0, -1.0), &lightRedPurple);
  Plane plane5(Vec3(-3.0, 0.0, 0.0), Vec3(1.0, 0.0, 0.0), &yellow);
  Plane plane6(Vec3(3.0, 0.0, 0.0), Vec3(-1.0, 0.0, 0.0), &yellow);

  std::cout << "Adding geometric objects" << std::endl;
  world.AddGeometricObject(&lightSphere);
  world.AddGeometricObject(&sphere2);
  world.AddGeometricObject(&sphere3);

  world.AddGeometricObject(&plane);
  world.AddGeometricObject(&plane3);
  world.AddGeometricObject(&plane5);
  world.AddGeometricObject(&plane6);

  world.AddLightSource(&lightSphere);

  time_t start;
  double seconds;
  Scene res;

  std::cout << "Rendering perspective scene" << std::endl;
  world.SetCamera(&cameraP);
  start = time(0);
  double yShift = 0.0;
  double gravity = -0.01;
  Vec3 position = lightSphere.Position();
  for (int i = 0; i < 32; i++) {
    yShift += gravity;
    position.y += yShift;
    lightSphere.SetPosition(position.x, position.y, position.z);
    res = world.Render(0);
    ToPNG(filename + std::to_string(i) + ".png", res);
  }
  seconds = difftime(time(0), start);
  std::cout << "Rendered perspective in " << seconds << " seconds" << std::endl;
}

void MotionBlur(const std::string& filename)
{
  /* Initialize world */
  std::cout << "Initializing world setting" << std::endl;
  World world = World();

  Vec3 cp(0,0, 10);
  Vec3 la(0,0,-1);
  Vec3 up(0,1,0);

  PerspectiveCamera cameraP(cp, la, up, 10);

  MultiJitteredSampler2D sampler = MultiJitteredSampler2D(2, 2);
  ViewPlane viewPlane(0.01,0.01,1080,960);

  std::cout << "Applying world setting" << std::endl;
  world.SetGeometricLayoutType(LayoutType::LIST);
  world.SetViewPlane(&viewPlane);
  world.SetSampler(&sampler);

  /* Initializing materials */
  Material green = Material(RGBColor(0.0,1.0,0.0));
  Material redPurple = Material(RGBColor(1.0,0.0,0.5));
  Material yellow = Material(RGBColor(0.5,1.0,1.0));
  Material white = Material(RGBColor(1.0, 1.0, 1.0));
  Material black = Material(RGBColor(0.5, 0.5, 0.5));

  Material whiteLightSource = Material();
  whiteLightSource.SimpleLightSource(RGBColor(1.0, 1.0, 1.0));

  Material transparentMaterial = Material();
  transparentMaterial.SimpleTransparentMaterial();

  std::cout << "Creating geometric objects" << std::endl;

  PointLight light(Vec3(0.0, 1.0, 2.0), RGBColor(1.0, 1.0, 1.0));

  Sphere sphere2(Vec3(-1.5, -2.0, 0.0), 1.0, &redPurple);
  Sphere sphere3(Vec3(1.5, -2.0, -1.0), 1.0, &green);
  Sphere movingSphere(Vec3(-2.5, 2.0, 5.0), 0.5, &redPurple);


  Plane plane(Vec3(0.0, -3.0, 0.0), Vec3(0.0, 1.0, 0.0), &white);
  Plane plane2(Vec3(0.0, 3.0, 0.0), Vec3(0.0, -1.0, 0.0), &redPurple);
  Plane plane3(Vec3(0.0, 0.0, -2.0), Vec3(0.0, 0.0, 1.0), &black);
  Plane plane5(Vec3(-3.0, 0.0, 0.0), Vec3(1.0, 0.0, 0.0), &yellow);
  Plane plane6(Vec3(3.0, 0.0, 0.0), Vec3(-1.0, 0.0, 0.0), &yellow);

  std::cout << "Adding geometric objects" << std::endl;
  world.AddGeometricObject(&sphere2);
  world.AddGeometricObject(&sphere3);
  world.AddGeometricObject(&movingSphere);

  world.AddGeometricObject(&plane);
  world.AddGeometricObject(&plane3);
  world.AddGeometricObject(&plane5);
  world.AddGeometricObject(&plane6);

  world.AddLightSource(&light);

  time_t start;
  double seconds;
  Scene res;
  Scene tempRes;

  std::cout << "Rendering perspective scene" << std::endl;
  world.SetCamera(&cameraP);
  start = time(0);
  Vec3 position = movingSphere.Position();
  double yShift = 0.0;
  double xShift = 0.15625;
  double zShift = 0.0625;
  double gravity = -0.01;
  for (int i = 0; i < 37; i++) {
    if (i == 28) {
      res = world.Render(0);
      gravity /= 2;
      xShift /= 2;
      yShift /= 2;
      zShift /= 2;
    }
    if (i > 28 && i <= 36) {
      tempRes = world.Render(0);
      for (int i = 0; i < res.size(); i++) {
        for (int j = 0; j < res[i].size(); j++) {
          res[i][j] = (res[i][j] / 2.0 + tempRes[i][j]) / 1.5;
        }
      }
    }
    if (i < 37) {
      position.x += xShift;
    } else {
      position.x -= xShift;
    }
    position.z -= zShift;
    if (i == 37) {
      yShift *= -1;
    }
    yShift += gravity;
    position.y += yShift;
    movingSphere.SetPosition(position.x, position.y, position.z);
    if (res.size() != 0) {
      ToPNG(filename + std::to_string(i) + ".png", res);
    }
  }
  seconds = difftime(time(0), start);
  std::cout << "Rendered perspective in " << seconds << " seconds" << std::endl;
}

void MotionBlurB(const std::string& filename)
{
  /* Initialize world */
  std::cout << "Initializing world setting" << std::endl;
  World world = World();

  Vec3 cp(0,0, 10);
  Vec3 la(0,0,-1);
  Vec3 up(0,1,0);

  PerspectiveCamera cameraP(cp, la, up, 10);

  MultiJitteredSampler2D sampler = MultiJitteredSampler2D(2, 2);
  ViewPlane viewPlane(0.02,0.02,540,480);

  std::cout << "Applying world setting" << std::endl;
  world.SetGeometricLayoutType(LayoutType::LIST);
  world.SetViewPlane(&viewPlane);
  world.SetSampler(&sampler);

  /* Initializing materials */
  Material green = Material(RGBColor(0.0,1.0,0.0));
  Material redPurple = Material(RGBColor(1.0,0.0,0.5));
  Material yellow = Material(RGBColor(0.5,1.0,1.0));
  Material white = Material(RGBColor(1.0, 1.0, 1.0));
  Material black = Material(RGBColor(0.5, 0.5, 0.5));

  Material whiteLightSource = Material();
  whiteLightSource.SimpleLightSource(RGBColor(1.0, 1.0, 1.0));

  Material transparentMaterial = Material();
  transparentMaterial.SimpleTransparentMaterial();

  std::cout << "Creating geometric objects" << std::endl;

  PointLight light(Vec3(0.0, 0.5, 2.0), RGBColor(1.0, 1.0, 1.0));
  SphereAreaLight lightSphere(Vec3(0.0, 0.5, 2.0), 0.1, &whiteLightSource, 8);


  Sphere sphere2(Vec3(-1.5, -2.0, 0.0), 1.0, &redPurple);
  Sphere sphere3(Vec3(1.5, -2.0, -1.0), 1.0, &green);
  Sphere movingSphere(Vec3(-2.5, 2.0, 5.0), 0.5, &redPurple);


  Plane plane(Vec3(0.0, -3.0, 0.0), Vec3(0.0, 1.0, 0.0), &white);
  Plane plane2(Vec3(0.0, 3.0, 0.0), Vec3(0.0, -1.0, 0.0), &redPurple);
  Plane plane3(Vec3(0.0, 0.0, -2.0), Vec3(0.0, 0.0, 1.0), &black);
  Plane plane5(Vec3(-3.0, 0.0, 0.0), Vec3(1.0, 0.0, 0.0), &yellow);
  Plane plane6(Vec3(3.0, 0.0, 0.0), Vec3(-1.0, 0.0, 0.0), &yellow);

  std::cout << "Adding geometric objects" << std::endl;
  world.AddGeometricObject(&lightSphere);
  world.AddGeometricObject(&sphere2);
  world.AddGeometricObject(&sphere3);
  world.AddGeometricObject(&movingSphere);

  world.AddGeometricObject(&plane);
  world.AddGeometricObject(&plane3);
  world.AddGeometricObject(&plane5);
  world.AddGeometricObject(&plane6);

  world.AddLightSource(&lightSphere);

  time_t start;
  double seconds;
  Scene res;
  Scene composedRes;
  Scene tempRes;
  double counter = 1.0;

  std::cout << "Rendering perspective scene" << std::endl;
  world.SetCamera(&cameraP);
  start = time(0);
  Vec3 position = movingSphere.Position();
  Vec3 lightPosition = lightSphere.Position();
  double lightYShift = 0.0;
  double lightGravity = 0.0;
  double yShift = 0.0;
  double xShift = 0.15625;
  double zShift = 0.0625;
  double gravity = -0.01;
  for (int i = 0; i < 37; i++) {
    if (i == 28) {
      res = world.Render(0);
      gravity /= 2;
      xShift /= 2;
      yShift /= 2;
      zShift /= 2;
      lightYShift = -0.05;
      lightGravity = gravity;
    }
    if (i > 28 && i <= 36) {
      tempRes = world.Render(0);
      for (int i = 0; i < res.size(); i++) {
        for (int j = 0; j < res[i].size(); j++) {
          res[i][j] = (res[i][j] * (counter / (counter + 1.0))  + tempRes[i][j] / (counter + 1.0));
        }
      }
      counter += 1.0;
    }
    if (i < 37) {
      position.x += xShift;
    } else {
      position.x -= xShift;
    }
    position.z -= zShift;
    if (i == 37) {
      yShift *= -1;
    }
    yShift += gravity;
    position.y += yShift;
    lightYShift += lightGravity;
    lightPosition.y += lightYShift;
    lightSphere.SetPosition(lightPosition.x, lightPosition.y, lightPosition.z);
    movingSphere.SetPosition(position.x, position.y, position.z);
    if (res.size() != 0) {
      ToPNG(filename + std::to_string(i) + ".png", res);
    }
  }
  seconds = difftime(time(0), start);
  std::cout << "Rendered perspective in " << seconds << " seconds" << std::endl;
}