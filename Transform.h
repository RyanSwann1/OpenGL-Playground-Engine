#pragma once

#include "glm/glm.hpp"

struct Transform
{
	Transform()
		: position(),
		rotation(),
		scale()
	{}

	Transform(const glm::vec3& position, glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 rotation = glm::vec3())
		: position(position),
		rotation(rotation),
		scale(scale)
	{}

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};