#include "SceneExporter.h"

#include "../external/png.h"
#include "../external/rgbapixel.h"

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
      RGBColor pixel = scene[x][y].To8Bit();
      *out(x, (height-1)-y) = RGBAPixel(pixel.x, pixel.y, pixel.z);
    }
  }
  out.writeToFile(filename);
}