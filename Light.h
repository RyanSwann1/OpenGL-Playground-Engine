#pragma once

#include "glm/glm.hpp"
#ifdef DEBUG
#include "Mesh.h"
#endif // DEBUG

class ShaderHandler;
struct Light
{
	Light(const glm::vec3& startingPosition, const glm::vec3& startingColor);

#ifdef DEBUG
	void render(ShaderHandler& shaderHandler);
#endif // DEBUG

	glm::vec3 position;
	glm::vec3 color;
#ifdef DEBUG
	Mesh mesh;
#endif // DEBUG
};