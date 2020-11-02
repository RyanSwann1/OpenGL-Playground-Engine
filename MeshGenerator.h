#pragma once

#include "glm/glm.hpp"

struct Mesh;
namespace MeshGenerator
{
	void generateCubeMesh(Mesh& mesh, const glm::vec3& position);
}