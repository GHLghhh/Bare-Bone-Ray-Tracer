#pragma once

#include <string>
#include "../World.h"

// [TODO] Fix the undefine reference problem here
class SceneExporter {
public:
  static void ToPNG(const std::string& filename, const Scene& scene);
};