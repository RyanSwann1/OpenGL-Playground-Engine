#pragma once

#ifdef DEBUG
#include "glm/glm.hpp"

struct Mesh;
namespace MeshGenerator
{
	void generateCubeMesh(Mesh& mesh, glm::vec3 position = glm::vec3(1.0f));
}
#endif // DEBUG