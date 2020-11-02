#pragma once

#include "glm/glm.hpp"
#ifdef DEBUG_LIGHTING
#include "Mesh.h"
#endif // DEBUG_LIGHTING

class ShaderHandler;
struct Light
{
	Light(const glm::vec3& startingPosition, const glm::vec3& startingColor);

#ifdef DEBUG_LIGHTING
	void render(ShaderHandler& shaderHandler);
#endif // DEBUG_LIGHTING

	glm::vec3 position;
	glm::vec3 color;
#ifdef DEBUG_LIGHTING
	Mesh mesh;
#endif // DEBUG_LIGHTING
};