#include <fstream>
#include <sstream>

#include "loadMeshes.h"

/*
 * Adopt from https://stackoverflow.com/questions/7868936/read-file-line-by-line-using-ifstream-in-c
 */
std::vector<Triangle> LoadFromObjFile(std::string fileName)
{
  std::vector<Triangle> meshes;
  std::vector<Vec3> vertices;

  std::ifstream inputFile(fileName);
  std::string line;
  while (std::getline(inputFile, line)) {
    std::istringstream iss(line);
    char type;
    double value1, value2, value3;
    if (!(iss >> type >> value1 >> value2 >> value3) && line.size() != 0) {
      // error
      std::cout << "Faile to parse: " << line << std::endl;
      break;
    }
    switch (type) {
      case 'v':
        {
          vertices.push_back(Vec3(value1, value2, value3));
        }
        break;
      case 'f':
        {
          Vec3 vertex1 = vertices[((int)value1)-1];
          Vec3 vertex2 = vertices[((int)value2)-1];
          Vec3 vertex3 = vertices[((int)value3)-1];
          meshes.push_back(Triangle(vertex1, vertex2, vertex3, RGBColor(1.0, 1.0, 1.0)));
        }
        break;
      default:
        {
          std::cout << "Read unknown type in obj file: " << type << std::endl;
        }
    }
  }

  return meshes;
}