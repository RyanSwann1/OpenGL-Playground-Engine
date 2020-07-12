#pragma once

#include "glm/glm.hpp"
#include <vector>

struct Mesh;
namespace MeshGenerator
{
	void generateCubeMesh(const glm::vec3& position, Mesh& mesh);
}