#pragma once

#include "glm/glm.hpp"
#ifdef DEBUG
#include "Mesh.h"
#endif // DEBUG

class ShaderHandler;
struct DirectionalLight
{
	DirectionalLight(ShaderHandler& shaderHandler, const glm::vec3& direction, const glm::vec3& color,
		float intensity);

	glm::vec3 direction;
	glm::vec3 color;
	float intensity;
};

struct Light
{
	Light(const glm::vec3& startingPosition, const glm::vec3& startingColor, float radius, float compression);

#ifdef DEBUG
	void render(ShaderHandler& shaderHandler) const;
#endif // DEBUG

	glm::vec3 position;
	glm::vec3 color;
	float radius;
	float compression;
#ifdef DEBUG
	Mesh mesh;
#endif // DEBUG
};