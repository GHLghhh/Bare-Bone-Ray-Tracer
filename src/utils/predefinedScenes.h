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

void MP2(const std::string& filename)
{
  // [TODO] fix later (generating random geometries v.s. fixed materials)

  std::cout << "Initializing world setting" << std::endl;
  World world = World();

  Vec3 cp = Vec3(0,0, 2);
  Vec3 la = Vec3(0,0,-1);
  Vec3 up = Vec3(0,1,0);

  Vec3 cpMesh = Vec3(0, 2.2, 3);
  Vec3 laMesh = Vec3(0,-0.1,-1);
  Vec3 upMesh = Vec3(0,1,0);
  PerspectiveCamera cameraP(cp, la, up, 2);
  PerspectiveCamera cameraPMesh(cpMesh, laMesh, upMesh, 2);
  OrthographicCamera cameraO(cp, la, up);

  ViewPlane viewPlane(0.01,0.01,1080,960);

  PointLight light(Vec3(5, 10, 0), Vec3(0.5, 1.0, 1.0));
  PointLight light2(Vec3(10, 0, 5), Vec3(1.0, 0.5, 0.5));
  PointLight light3(Vec3(-1, -3, 3), Vec3(0.5, 1.0, 0.5));

  MultiJitteredSampler2D samplerM = MultiJitteredSampler2D(4, 4);

  std::cout << "Applying world setting" << std::endl;
  world.SetGeometricLayoutType(LayoutType::GRID);
  world.SetViewPlane(&viewPlane);
  world.SetSampler(&samplerM);
  world.SetCamera(&cameraP);
  world.AddLightSource(&light);
  world.AddLightSource(&light2);

  /* Generate spheres with different order of magnitude */
  Material white = Material(RGBColor(1.0, 1.0, 1.0));
  std::cout << "Creating geometric objects" << std::endl;
  std::vector<Sphere> spheres;
  for (int i = 0; i < 10000; i++) {
    Vec3 randomPosition = Vec3(((double) rand() / (RAND_MAX) - 0.5) * 40, ((double) rand() / (RAND_MAX) - 0.5) * 40, ((double) rand() / (RAND_MAX) + 0.3) * -20);
    double randomRadius = ((double) rand() / (RAND_MAX) + 0.1);
    // RGBColor randomColor = RGBColor(((double) rand() / (RAND_MAX)),((double) rand() / (RAND_MAX)),((double) rand() / (RAND_MAX)));
    spheres.emplace_back(randomPosition, randomRadius, &white);
  }

  /* Results from grid layout */
  for (int i = 0; i < spheres.size(); i++) {
    world.AddGeometricObject(&spheres[i]);
    if (i == 99) {
      std::cout << "Rendering 100 sphere with grid" << std::endl;
      time_t start = time(0);
      Scene res = world.Render();
      double seconds = difftime(time(0), start);
      ToPNG("outs/1Hballs_grid.png", res);
      std::cout << "Finished 1h balls with grid in " << seconds << std::endl;
    }
    if (i == 999) {
      std::cout << "Rendering 1000 sphere with grid" << std::endl;
      time_t start = time(0);
      Scene res = world.Render();
      double seconds = difftime(time(0), start);
      ToPNG("outs/1kballs_grid.png", res);
      std::cout << "Finished 1k balls with grid in " << seconds << std::endl;
    }
    if (i == 9999) {
      std::cout << "Rendering 10000 sphere with grid" << std::endl;
      time_t start = time(0);
      Scene res = world.Render();
      double seconds = difftime(time(0), start);
      ToPNG("outs/1wballs_grid.png", res);
      std::cout << "Finished 1w balls with grid in " << seconds << std::endl;
    }
  }

  /* Results from list layout (no acceleration) */
  world.DiscardGeometricObjects();
  world.SetGeometricLayoutType(LayoutType::LIST);
  for (int i = 0; i < spheres.size(); i++) {
    world.AddGeometricObject(&spheres[i]);
    if (i == 99) {
      std::cout << "Rendering 100 sphere with list" << std::endl;
      time_t start = time(0);
      Scene res = world.Render();
      double seconds = difftime(time(0), start);
      ToPNG(filename + "1Hballs_list.png", res);
      std::cout << "Finished 1h balls with list in " << seconds << std::endl;
    }
    if (i == 999) {
      std::cout << "Rendering 1000 sphere with list" << std::endl;
      time_t start = time(0);
      Scene res = world.Render();
      double seconds = difftime(time(0), start);
      ToPNG(filename + "1kballs_list.png", res);
      std::cout << "Finished 1k balls with list in " << seconds << std::endl;
    }
    if (i == 9999) {
      std::cout << "Rendering 10000 sphere with list" << std::endl;
      time_t start = time(0);
      Scene res = world.Render();
      double seconds = difftime(time(0), start);
      ToPNG(filename + "10kballs_list.png", res);
      std::cout << "Finished 1w balls with list in " << seconds << std::endl;
    }
  }

  /* Setup for mesh */
  std::cout << "Loading geometric objects from obj file" << std::endl;
  std::vector<Triangle> meshes = LoadFromObjFile("teapot.obj");
  std::cout << meshes.size() << std::endl;
  std::cout << "End loading meshes" << std::endl;

  std::cout << "Adding geometric objects" << std::endl;
  world.DiscardGeometricObjects();
  for (size_t i = 0; i < meshes.size(); i++) {
    world.AddGeometricObject(&meshes[i]);
  }
  std::cout << "Finished adding meshes" << std::endl;

  world.SetCamera(&cameraPMesh);
  world.AddLightSource(&light3);

  std::cout << "Rendering teapot with list" << std::endl;
  time_t start = time(0);
  Scene res = world.Render();
  double seconds = difftime(time(0), start);
  ToPNG(filename + "teapot_list.png", res);
  std::cout << "Finished bunny with list in " << seconds << std::endl;

  world.SetGeometricLayoutType(LayoutType::GRID);

  std::cout << "Rendering teapot with grid" << std::endl;
  start = time(0);
  res = world.Render();
  seconds = difftime(time(0), start);
  ToPNG(filename + "teapot_grid.png", res);
  std::cout << "Finished bunny with grid in " << seconds << std::endl;
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

void MP5(const std::string& filename)
{
  /* Initialize world */
  std::cout << "Initializing world setting" << std::endl;
  World world = World();

  Vec3 cp(278, 273, -800);
  Vec3 la(0,0,1);
  Vec3 up(0,1,0);

  PerspectiveCamera cameraP(cp, la, up, 800);

  Sampler2D sampler = Sampler2D();
  ViewPlane viewPlane(5,5,100,100);

  std::cout << "Applying world setting" << std::endl;
  world.SetGeometricLayoutType(LayoutType::GRID);
  world.SetViewPlane(&viewPlane);
  world.SetSampler(&sampler);

  /* Initializing materials */
  Material green = Material(RGBColor(0.0,1.0,0.0));
  Material red = Material(RGBColor(1.0,0.0,0.0));
  Material white = Material(RGBColor(1.0, 1.0, 1.0));

  Material whiteLightSource = Material();
  whiteLightSource.SimpleLightSource(RGBColor(1.0, 0.73, 0.4));

  std::cout << "Creating geometric objects" << std::endl;

  // [TODO] remove the temp light and geometries after area light is implemented
  // cos term that effective?
  PointLight light(Vec3(226.0, 400.0, 180.0), RGBColor(1.0, 1.0, 1.0));
  Plane back(Vec3(278.0, 273.0, 0), Vec3(0.0, 0.0, -1.0), &white);

  // Light (slightly lower than ceiling in y direction)
  // 343.0 548.8 227.0 
  // 343.0 548.8 332.0
  // 213.0 548.8 332.0
  // 213.0 548.8 227.0
  // [TODO] Create square geometric object to simplify sampling
  //        or figure out a way to sample triangle uniformly
  TriangleAreaLight light1(
    Vec3(343.0, 548.7, 227.0), Vec3(343.0, 548.7, 332.0),
    Vec3(213.0, 548.7, 332.0), &whiteLightSource, 4);
  TriangleAreaLight light2(
    Vec3(213.0, 548.7, 332.0), Vec3(213.0, 548.7, 227.0),
    Vec3(343.0, 548.7, 227.0), &whiteLightSource, 4);

  // Floor
  // 552.8 0.0   0.0   
  // 0.0 0.0   0.0
  // 0.0 0.0 559.2
  // 549.6 0.0 559.2
  Triangle floor1(
    Vec3(552.8, 0.0, 0.0), Vec3(0.0, 0.0, 0.0),
    Vec3(0.0, 0.0, 559.2), &white);
  Triangle floor2(
    Vec3(0.0, 0.0, 559.2), Vec3(549.6, 0.0, 559.2),
    Vec3(552.8, 0.0, 0.0), &white);

  // Ceiling
  // 556.0 548.8 0.0   
  // 556.0 548.8 559.2
  // 0.0 548.8 559.2
  // 0.0 548.8   0.0
  Triangle ceil1(
    Vec3(556.0, 548.8, 0.0), Vec3(556.0, 548.8, 559.2),
    Vec3(0.0, 548.8, 559.2), &white);
  Triangle ceil2(
    Vec3(0.0, 548.8, 559.2), Vec3(0.0, 548.8, 0.0),
    Vec3(556.0, 548.8, 0.0), &white);

  // Back wall
  // 549.6   0.0 559.2 
  // 0.0   0.0 559.2
  // 0.0 548.8 559.2
  // 556.0 548.8 559.2
  Triangle back1(
    Vec3(549.6, 0.0, 559.2), Vec3(0.0, 0.0, 559.2),
    Vec3(0.0, 548.8, 559.2), &white);
  Triangle back2(
    Vec3(0.0, 548.8, 559.2), Vec3(556.0, 548.8, 559.2),
    Vec3(549.6, 0.0, 559.2), &white);

  // Right wall
  // 0.0   0.0 559.2   
  // 0.0   0.0   0.0
  // 0.0 548.8   0.0
  // 0.0 548.8 559.2
  Triangle right1(
    Vec3(0.0, 0.0, 559.2), Vec3(0.0, 0.0, 0.0),
    Vec3(0.0, 548.8, 0.0), &green);
  Triangle right2(
    Vec3(0.0, 548.8, 0.0), Vec3(0.0, 548.8, 559.2),
    Vec3(0.0, 0.0, 559.2), &green);

  // Left wall
  // 552.8   0.0   0.0 
  // 549.6   0.0 559.2
  // 556.0 548.8 559.2
  // 556.0 548.8   0.0
  Triangle left1(
    Vec3(552.8, 0.0, 0.0), Vec3(549.6, 0.0, 559.2),
    Vec3(556.0, 548.8, 559.2), &red);
  Triangle left2(
    Vec3(556.0, 548.8, 559.2), Vec3(556.0, 548.8, 0.0),
    Vec3(552.8, 0.0, 0.0), &red);

  // Short block
  // 130.0 165.0  65.0 
  // 82.0 165.0 225.0
  // 240.0 165.0 272.0
  // 290.0 165.0 114.0
  Triangle sba1(
    Vec3(130.0, 165.0, 65.0), Vec3(82.0, 165.0, 225.0),
    Vec3(240.0, 165.0, 272.0), &white);
  Triangle sba2(
    Vec3(240.0, 165.0, 272.0), Vec3(290.0, 165.0, 114.0),
    Vec3(130.0, 165.0, 65.0), &white);

  // 290.0   0.0 114.0
  // 290.0 165.0 114.0
  // 240.0 165.0 272.0
  // 240.0   0.0 272.0
  Triangle sbb1(
    Vec3(290.0, 0.0, 114.0), Vec3(290.0, 165.0, 114.0),
    Vec3(240.0, 165.0, 272.0), &white);
  Triangle sbb2(
    Vec3(240.0, 165.0, 272.0), Vec3(240.0, 0.0, 272.0),
    Vec3(290.0, 0.0, 114.0), &white);

  // 130.0   0.0  65.0
  // 130.0 165.0  65.0
  // 290.0 165.0 114.0
  // 290.0   0.0 114.0
  Triangle sbc1(
    Vec3(130.0, 0.0, 65.0), Vec3(130.0, 165.0, 65.0),
    Vec3(290.0, 165.0, 114.0), &white);
  Triangle sbc2(
    Vec3(290.0, 165.0, 114.0), Vec3(290.0, 0.0, 114.0),
    Vec3(130.0, 0.0, 65.0), &white);

  // 82.0   0.0 225.0
  // 82.0 165.0 225.0
  // 130.0 165.0  65.0
  // 130.0   0.0  65.0
  Triangle sbd1(
    Vec3(82.0, 0.0, 225.0), Vec3(82.0, 165.0, 225.0),
    Vec3(130.0, 165.0, 65.0), &white);
  Triangle sbd2(
    Vec3(130.0, 165.0, 65.0), Vec3(130.0, 0.0, 65.0),
    Vec3(82.0, 0.0, 225.0), &white);

  // 240.0   0.0 272.0
  // 240.0 165.0 272.0
  // 82.0 165.0 225.0
  // 82.0   0.0 225.0
  Triangle sbe1(
    Vec3(240.0, 0.0, 272.0), Vec3(240.0, 165.0, 272.0),
    Vec3(82.0, 165.0, 225.0), &white);
  Triangle sbe2(
    Vec3(82.0, 165.0, 225.0), Vec3(82.0, 0.0, 225.0),
    Vec3(240.0, 0.0, 272.0), &white);

  // 423.0 330.0 247.0
  // 265.0 330.0 296.0
  // 314.0 330.0 456.0
  // 472.0 330.0 406.0
  Triangle tba1(
    Vec3(423.0, 330.0, 247.0), Vec3(265.0, 330.0, 296.0),
    Vec3(314.0, 330.0, 456.0), &white);
  Triangle tba2(
    Vec3(314.0, 330.0, 456.0), Vec3(472.0, 330.0, 406.0),
    Vec3(423.0, 330.0, 247.0), &white);

  // 423.0   0.0 247.0
  // 423.0 330.0 247.0
  // 472.0 330.0 406.0
  // 472.0   0.0 406.0
  Triangle tbb1(
    Vec3(423.0, 0.0, 247.0), Vec3(423.0, 330.0, 247.0),
    Vec3(472.0, 330.0, 406.0), &white);
  Triangle tbb2(
    Vec3(472.0, 330.0, 406.0), Vec3(472.0, 0.0, 406.0),
    Vec3(423.0, 0.0, 247.0), &white);

  // 472.0   0.0 406.0
  // 472.0 330.0 406.0
  // 314.0 330.0 456.0
  // 314.0   0.0 456.0
  Triangle tbc1(
    Vec3(472.0, 0.0, 406.0), Vec3(472.0, 330.0, 406.0),
    Vec3(314.0, 330.0, 456.0), &white);
  Triangle tbc2(
    Vec3(314.0, 330.0, 456.0), Vec3(314.0, 0.0, 456.0),
    Vec3(472.0, 0.0, 406.0), &white);

  // 314.0   0.0 456.0
  // 314.0 330.0 456.0
  // 265.0 330.0 296.0
  // 265.0   0.0 296.0
  Triangle tbd1(
    Vec3(314.0, 0.0, 456.0), Vec3(314.0, 330.0, 456.0),
    Vec3(265.0, 330.0, 296.0), &white);
  Triangle tbd2(
    Vec3(265.0, 330.0, 296.0), Vec3(265.0, 0.0, 296.0),
    Vec3(314.0, 0.0, 456.0), &white);

  // 265.0   0.0 296.0
  // 265.0 330.0 296.0
  // 423.0 330.0 247.0
  // 423.0   0.0 247.0
  Triangle tbe1(
    Vec3(265.0, 0.0, 296.0), Vec3(265.0, 330.0, 296.0),
    Vec3(423.0, 330.0, 247.0), &white);
  Triangle tbe2(
    Vec3(423.0, 330.0, 247.0), Vec3(423.0, 0.0, 247.0),
    Vec3(265.0, 0.0, 296.0), &white);

  std::cout << "Adding geometric objects" << std::endl;
  world.AddGeometricObject(&light1);
  world.AddGeometricObject(&light2);

  world.AddGeometricObject(&floor1);
  world.AddGeometricObject(&floor2);
  world.AddGeometricObject(&ceil1);
  world.AddGeometricObject(&ceil2);
  world.AddGeometricObject(&back1);
  world.AddGeometricObject(&back2);

  world.AddGeometricObject(&right1);
  world.AddGeometricObject(&right2);
  world.AddGeometricObject(&left1);
  world.AddGeometricObject(&left2);

  world.AddGeometricObject(&sba1);
  world.AddGeometricObject(&sba2);
  world.AddGeometricObject(&sbb1);
  world.AddGeometricObject(&sbb2);
  world.AddGeometricObject(&sbc1);
  world.AddGeometricObject(&sbc2);
  world.AddGeometricObject(&sbd1);
  world.AddGeometricObject(&sbd2);
  world.AddGeometricObject(&sbe1);
  world.AddGeometricObject(&sbe2);

  world.AddGeometricObject(&tba1);
  world.AddGeometricObject(&tba2);
  world.AddGeometricObject(&tbb1);
  world.AddGeometricObject(&tbb2);
  world.AddGeometricObject(&tbc1);
  world.AddGeometricObject(&tbc2);
  world.AddGeometricObject(&tbd1);
  world.AddGeometricObject(&tbd2);
  world.AddGeometricObject(&tbe1);
  world.AddGeometricObject(&tbe2);

  // world.AddGeometricObject(&back);

  // world.AddLightSource(&light);
  world.AddLightSource(&light1);
  world.AddLightSource(&light2);

  time_t start;
  double seconds;
  Scene res;

  std::cout << "Rendering perspective scene" << std::endl;
  world.SetCamera(&cameraP);
  start = time(0);
  res = world.Render(10);
  ToPNG(filename + ".png", res);
  // Highlight cache point
  // Note that this is using scene knowledge, not general knowledge
  std::vector<Vec3*> cachePoints = world.GetCachePoints();
  double viewPlaneWidth = viewPlane.PixelWidth() * viewPlane.HorizontalResolution();
  double viewPlaneHeight = viewPlane.PixelHeight() * viewPlane.VerticalResolution();
  double planeux = cp.x - viewPlaneWidth / 2;
  double planelx = cp.x + viewPlaneWidth / 2;
  double planely = cp.y - viewPlaneHeight / 2;
  double planeuy = cp.y + viewPlaneHeight / 2;
  
  std::cout << "size " << cachePoints.size() << std::endl;
  for (auto& point_ptr : cachePoints) {
    Vec3 rayDirection = (*point_ptr) - cp;
    // know z location of the viewplane.
    double t = 800.0 / rayDirection.z;
    double x = cp.x + t * rayDirection.x;
    double y = cp.y + t * rayDirection.y;
    int xIdx = fabs(((x - planelx) / viewPlaneWidth) * viewPlane.HorizontalResolution());
    int yIdx = fabs(((y - planely) / viewPlaneHeight) * viewPlane.VerticalResolution());
    res[xIdx][yIdx] = RGBColor(0.0, 0.0, 1.0);
  }
  ToPNG(filename + "_highlight.png", res);
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