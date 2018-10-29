#pragma once

#include "../Vec3.h"
#include "../../geometricObjects/Triangle.h"

#include <vector>
#include <iostream>

std::vector<Triangle> LoadFromObjFile(std::string fileName);