#pragma once

#include <iostream>
#include <vector>

#include "../Vec3.h"
#include "../../geometricObjects/Triangle.h"

std::vector<Triangle> LoadFromObjFile(std::string fileName);