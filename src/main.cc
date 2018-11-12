#include <iostream>
#include <time.h>

#include "thirdParty/png.h"
#include "thirdParty/rgbapixel.h"

#include "Ray.h"
#include "World.h"
#include "geometricObjects/Plane.h"
#include "geometricObjects/Sphere.h"
#include "geometricObjects/Triangle.h"
#include "materials/Material.h"
#include "samplers/MultiJitteredSampler2D.h"
#include "samplers/Sampler2D.h"
#include "utils/io/loadMeshes.h"
#include "utils/RGBColor.h"
#include "utils/Vec3.h"

static bool DEBUG_BUILD = true;

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
  /* Initialize world */
  World world = World();

  if (DEBUG_BUILD) {
    std::cout << "Initializing world setting" << std::endl;
    Vec3 cp(0,0, 2);
    Vec3 la(0,0,-1);
    Vec3 up(0,1,0);

    PerspectiveCamera cameraP(cp, la, up, 2);
    OrthographicCamera cameraO(cp, la, up);
    PointLight light(Vec3(5, 10, 0), RGBColor(0.5, 1.0, 1.0));
    std::cout << "light: " << &light << std::endl;
    PointLight light2(Vec3(10, 0, 5), RGBColor(1.0, 0.5, 0.5));
    std::cout << "light2: " << &light2 << std::endl;

    Sampler2D sampler = Sampler2D();
    ViewPlane viewPlane(0.01,0.01,1080,960);

    std::cout << "Applying world setting" << std::endl;
    world.SetGeometricLayoutType(LayoutType::GRID);
    world.SetViewPlane(&viewPlane);
    world.SetSampler(&sampler);
    // world.AddLightSource(&light);
    // world.AddLightSource(&light2);

    /* Testing set */
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
    // Triangle triangle1(Vec3(-1, -1, 0), Vec3(1, -1, 0), Vec3(1, 1, 0),
    //   &green);
    // Triangle triangle1_(Vec3(1, 1, 0), Vec3(1, -1, 0), Vec3(-1, -1, 0),
    //   &green);
    Sphere sph3(Vec3(-3.5, -2.0, -1.0), 1.0, &green);
    // std::cout << "tri1: " << &triangle1 << std::endl;
    Triangle triangle2(Vec3(2, 0, 0), Vec3(2, -3, -1), Vec3(0, -2, 1),
      &redPurple);
    Triangle triangle2_(Vec3(0, -2, 1), Vec3(2, -3, -1), Vec3(2, 0, 0),
      &redPurple);
    // std::cout << "tri2: " << &triangle2 << std::endl;

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

    // Sphere sphere1(Vec3(-1, 0.75, 0), 1.0, RGBColor(1.0,1.0,1.0));
    SphereAreaLight sphere1(Vec3(-1, 1, 1), 0.5, &whiteLightSource, 64);
    std::cout << "sph1: " << &sphere1 << std::endl;
    Sphere sphere2(Vec3(-1.5, -1.5, 0.0), 1.0, &transparentMaterial);
    std::cout << "sph2: " << &sphere2 << std::endl;
    
    std::cout << "Adding geometric objects" << std::endl;
    // world.AddGeometricObject(&triangle1);
    world.AddGeometricObject(&triangle2);
    // world.AddGeometricObject(&triangle1_);
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
    // world.AddGeometricObject(&plane5);
    // world.AddGeometricObject(&plane6);

    world.AddLightSource(&sphere1);

    // // [TODO] check if orthographic camera is shaded correctly.
    time_t start;
    double seconds;
    Scene res;

    // std::cout << "Rendering orthographic scene" << std::endl;
    // world.SetCamera(&cameraO);
    // start = time(0);
    // res = world.Render();
    // seconds = difftime(time(0), start);
    // ToPNG("outs/orthographic.png", res);
    // std::cout << "Rendered orthographic in " << seconds << " seconds"<< std::endl;

    std::cout << "Rendering perspective scene" << std::endl;
    world.SetCamera(&cameraP);
    start = time(0);
    res = world.Render();
    ToPNG("outs/perspective.png", res);
    seconds = difftime(time(0), start);
    std::cout << "Rendered perspective in " << seconds << " seconds" << std::endl;
  } else {
    // [TODO] fix later (generating random geometries v.s. fixed materials)

    // std::cout << "Initializing world setting" << std::endl;
    // Vec3 cp = Vec3(0,0, 2);
    // Vec3 la = Vec3(0,0,-1);
    // Vec3 up = Vec3(0,1,0);

    // Vec3 cpMesh = Vec3(0, 2.2, 3);
    // Vec3 laMesh = Vec3(0,-0.1,-1);
    // Vec3 upMesh = Vec3(0,1,0);
    // PerspectiveCamera cameraP(cp, la, up, 2);
    // PerspectiveCamera cameraPMesh(cpMesh, laMesh, upMesh, 2);
    // OrthographicCamera cameraO(cp, la, up);

    // ViewPlane viewPlane(0.01,0.01,1080,960);

    // PointLight light(Vec3(5, 10, 0), Vec3(0.5, 1.0, 1.0));
    // PointLight light2(Vec3(10, 0, 5), Vec3(1.0, 0.5, 0.5));
    // PointLight light3(Vec3(-1, -3, 3), Vec3(0.5, 1.0, 0.5));

    // MultiJitteredSampler2D samplerM = MultiJitteredSampler2D(4, 4);

    // std::cout << "Applying world setting" << std::endl;
    // world.SetGeometricLayoutType(LayoutType::GRID);
    // world.SetViewPlane(&viewPlane);
    // world.SetSampler(&samplerM);
    // world.SetCamera(&cameraP);
    // world.AddLightSource(&light);
    // world.AddLightSource(&light2);

    // /* Generate spheres with different order of magnitude */
    // std::cout << "Creating geometric objects" << std::endl;
    // std::vector<Sphere> spheres;
    // for (int i = 0; i < 10000; i++) {
    //   Vec3 randomPosition = Vec3(((double) rand() / (RAND_MAX) - 0.5) * 40, ((double) rand() / (RAND_MAX) - 0.5) * 40, ((double) rand() / (RAND_MAX) + 0.3) * -20);
    //   double randomRadius = ((double) rand() / (RAND_MAX) + 0.1);
    //   RGBColor randomColor = RGBColor(((double) rand() / (RAND_MAX)),((double) rand() / (RAND_MAX)),((double) rand() / (RAND_MAX)));
    //   spheres.emplace_back(randomPosition, randomRadius, randomColor);
    // }

    // /* Results from grid layout */
    // for (int i = 0; i < spheres.size(); i++) {
    //   world.AddGeometricObject(&spheres[i]);
    //   if (i == 99) {
    //     std::cout << "Rendering 100 sphere with grid" << std::endl;
    //     time_t start = time(0);
    //     Scene res = world.Render();
    //     double seconds = difftime(time(0), start);
    //     ToPNG("outs/1Hballs_grid.png", res);
    //     std::cout << "Finished 1h balls with grid in " << seconds << std::endl;
    //   }
    //   if (i == 999) {
    //     std::cout << "Rendering 1000 sphere with grid" << std::endl;
    //     time_t start = time(0);
    //     Scene res = world.Render();
    //     double seconds = difftime(time(0), start);
    //     ToPNG("outs/1kballs_grid.png", res);
    //     std::cout << "Finished 1k balls with grid in " << seconds << std::endl;
    //   }
    //   if (i == 9999) {
    //     std::cout << "Rendering 10000 sphere with grid" << std::endl;
    //     time_t start = time(0);
    //     Scene res = world.Render();
    //     double seconds = difftime(time(0), start);
    //     ToPNG("outs/1wballs_grid.png", res);
    //     std::cout << "Finished 1w balls with grid in " << seconds << std::endl;
    //   }
    // }

    // /* Results from list layout (no acceleration) */
    // world.DiscardGeometricObjects();
    // world.SetGeometricLayoutType(LayoutType::LIST);
    // for (int i = 0; i < spheres.size(); i++) {
    //   world.AddGeometricObject(&spheres[i]);
    //   if (i == 99) {
    //     std::cout << "Rendering 100 sphere with list" << std::endl;
    //     time_t start = time(0);
    //     Scene res = world.Render();
    //     double seconds = difftime(time(0), start);
    //     ToPNG("outs/1Hballs_list.png", res);
    //     std::cout << "Finished 1h balls with list in " << seconds << std::endl;
    //   }
    //   if (i == 999) {
    //     std::cout << "Rendering 1000 sphere with list" << std::endl;
    //     time_t start = time(0);
    //     Scene res = world.Render();
    //     double seconds = difftime(time(0), start);
    //     ToPNG("outs/1kballs_list.png", res);
    //     std::cout << "Finished 1k balls with list in " << seconds << std::endl;
    //   }
    //   if (i == 9999) {
    //     std::cout << "Rendering 10000 sphere with list" << std::endl;
    //     time_t start = time(0);
    //     Scene res = world.Render();
    //     double seconds = difftime(time(0), start);
    //     ToPNG("outs/1wballs_list.png", res);
    //     std::cout << "Finished 1w balls with list in " << seconds << std::endl;
    //   }
    // }

    // /* Setup for mesh */
    // std::cout << "Loading geometric objects from obj file" << std::endl;
    // std::vector<Triangle> meshes = LoadFromObjFile("teapot.obj");
    // std::cout << meshes.size() << std::endl;
    // std::cout << "End loading meshes" << std::endl;

    // std::cout << "Adding geometric objects" << std::endl;
    // world.DiscardGeometricObjects();
    // for (size_t i = 0; i < meshes.size(); i++) {
    //   world.AddGeometricObject(&meshes[i]);
    // }
    // std::cout << "Finished adding meshes" << std::endl;

    // world.SetCamera(&cameraPMesh);
    // world.AddLightSource(&light3);

    // std::cout << "Rendering teapot with list" << std::endl;
    // time_t start = time(0);
    // Scene res = world.Render();
    // double seconds = difftime(time(0), start);
    // ToPNG("outs/teapot_list.png", res);
    // std::cout << "Finished bunny with list in " << seconds << std::endl;

    // world.SetGeometricLayoutType(LayoutType::GRID);

    // std::cout << "Rendering teapot with grid" << std::endl;
    // start = time(0);
    // res = world.Render();
    // seconds = difftime(time(0), start);
    // ToPNG("outs/teapot_grid.png", res);
    // std::cout << "Finished bunny with grid in " << seconds << std::endl;
  }

  return 0;
}
