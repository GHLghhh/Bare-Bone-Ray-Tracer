#include <iostream>
#include "Ray.h"
#include "geometricObjects/Plane.h"
#include "geometricObjects/Triangle.h"
#include "geometricObjects/Sphere.h"
#include "utils/Vec3.h"
#include "utils/RGBColor.h"
#include "utils/ShadeRec.h"
#include "utils/io/loadMeshes.h"
#include "World.h"
#include "samplers/MultiJitteredSampler2D.h"

#include "external/png.h"
#include "external/rgbapixel.h"

#include <time.h>

static bool DEBUG = true;

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

  if (DEBUG) {
    Vec3 cp(0,0, 2);
    Vec3 la(0,0,-1);
    Vec3 up(0,1,0);

    PerspectiveCamera cameraP(cp, la, up, 2);
    OrthographicCamera cameraO(cp, la, up);

    PointLight light(Vec3(5, 10, 0), RGBColor(0.5, 1.0, 1.0));
    PointLight light2(Vec3(10, 0, 5), RGBColor(1.0, 0.5, 0.5));

    Sampler2D sampler = Sampler2D();
    ViewPlane viewPlane(0.01,0.01,108,96);
    
    world.SetGeometricLayoutType(LayoutType::GRID);
    world.SetViewPlane(&viewPlane);
    world.SetSampler(&sampler);
    world.AddLightSource(&light);
    world.AddLightSource(&light2);

    /* Testing set */
    Triangle triangle1(Vec3(-1, -1, 0), Vec3(1, -1, 0), Vec3(1, 1, 0),
      RGBColor(0.0,1.0,0.0));
    Triangle triangle2(Vec3(0, -2, 1), Vec3(2, -3, -1), Vec3(2, 0, 0),
      RGBColor(1.0,0.0,0.5));

    Sphere sphere1(Vec3(-1, 0.75, 0), 1.0, RGBColor(1.0,1.0,1.0));
    Sphere sphere2(Vec3(-1, -0.75, 0), 1.0, RGBColor(1.0,0.5,1.0));
    Sphere sphere3(Vec3(-0.0269652,0.795313,-1.81842), 1.0, RGBColor(1.0,0.5,1.0));
    

    world.AddGeometricObject(&triangle1);
    world.AddGeometricObject(&triangle2);
    world.AddGeometricObject(&sphere1);
    world.AddGeometricObject(&sphere2);
    // world.AddGeometricObject(&sphere3);

    // // [TODO] check if orthographic camera is shaded correctly.
    world.SetCamera(&cameraO);
    time_t start = time(0);
    Scene res = world.Render();
    double seconds = difftime(time(0), start);
    ToPNG("outs/orthographic.png", res);
    std::cout << "Rendered orthographic in " << seconds << " seconds"<< std::endl;

    world.SetCamera(&cameraP);
    start = time(0);
    res = world.Render();
    ToPNG("outs/perspective.png", res);
    seconds = difftime(time(0), start);
    std::cout << "Rendered perspective in " << seconds << " seconds" << std::endl;
  } else {
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

    world.SetGeometricLayoutType(LayoutType::GRID);
    world.SetViewPlane(&viewPlane);
    world.SetSampler(&samplerM);
    world.SetCamera(&cameraP);
    world.AddLightSource(&light);
    world.AddLightSource(&light2);

    /* Generate spheres with different order of magnitude */
    std::vector<Sphere> spheres;
    for (int i = 0; i < 10000; i++) {
      Vec3 randomPosition = Vec3(((double) rand() / (RAND_MAX) - 0.5) * 40, ((double) rand() / (RAND_MAX) - 0.5) * 40, ((double) rand() / (RAND_MAX) + 0.3) * -20);
      double randomRadius = ((double) rand() / (RAND_MAX) + 0.1);
      RGBColor randomColor = RGBColor(((double) rand() / (RAND_MAX)),((double) rand() / (RAND_MAX)),((double) rand() / (RAND_MAX)));
      spheres.emplace_back(randomPosition, randomRadius, randomColor);
    }

    /* Results from grid layout */
    for (int i = 0; i < spheres.size(); i++) {
      world.AddGeometricObject(&spheres[i]);
      if (i == 99) {
        time_t start = time(0);
        Scene res = world.Render();
        double seconds = difftime(time(0), start);
        ToPNG("outs/1Hballs_grid.png", res);
        std::cout << "Finished 1h balls with grid in " << seconds << std::endl;
      }
      if (i == 999) {
        time_t start = time(0);
        Scene res = world.Render();
        double seconds = difftime(time(0), start);
        ToPNG("outs/1kballs_grid.png", res);
        std::cout << "Finished 1k balls with grid in " << seconds << std::endl;
      }
      if (i == 9999) {
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
        time_t start = time(0);
        Scene res = world.Render();
        double seconds = difftime(time(0), start);
        ToPNG("outs/1Hballs_list.png", res);
        std::cout << "Finished 1h balls with list in " << seconds << std::endl;
      }
      if (i == 999) {
        time_t start = time(0);
        Scene res = world.Render();
        double seconds = difftime(time(0), start);
        ToPNG("outs/1kballs_list.png", res);
        std::cout << "Finished 1k balls with list in " << seconds << std::endl;
      }
      if (i == 9999) {
        time_t start = time(0);
        Scene res = world.Render();
        double seconds = difftime(time(0), start);
        ToPNG("outs/1wballs_list.png", res);
        std::cout << "Finished 1w balls with list in " << seconds << std::endl;
      }
    }

    /* Setup for mesh */
    std::vector<Triangle> meshes = LoadFromObjFile("teapot.obj");
    std::cout << meshes.size() << std::endl;
    std::cout << "End loading meshes" << std::endl;

    world.DiscardGeometricObjects();
    for (size_t i = 0; i < meshes.size(); i++) {
      world.AddGeometricObject(&meshes[i]);
    }
    std::cout << "Finished adding meshes" << std::endl;

    world.SetCamera(&cameraPMesh);
    world.AddLightSource(&light3);
    time_t start = time(0);

    Scene res = world.Render();
    double seconds = difftime(time(0), start);
    ToPNG("outs/teapot_list.png", res);
    std::cout << "Finished bunny with list in " << seconds << std::endl;

    world.SetGeometricLayoutType(LayoutType::GRID);

    start = time(0);
    res = world.Render();
    seconds = difftime(time(0), start);
    ToPNG("outs/teapot_grid.png", res);
    std::cout << "Finished bunny with grid in " << seconds << std::endl;
  }

  return 0;
}
