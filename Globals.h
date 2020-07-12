#pragma once

#include "glm/glm.hpp"
#include <array>

namespace Globals
{
	constexpr unsigned int INVALID_OPENGL_ID = 0;
	constexpr float CUBE_FACE_SIZE = 1.0f;

	constexpr std::array<glm::vec2, 6> QUAD_COORDS = 
	{
		glm::vec2(-0.5f, -0.5f),
		glm::vec2(0.5f, -0.5f),
		glm::vec2(0.5f, 0.5f),
		glm::vec2(0.5f, 0.5f),
		glm::vec2(-0.5f, 0.5f),
		glm::vec2(-0.5f, -0.5f)
	};

	constexpr std::array<glm::vec2, 6> TEXT_COORDS = 
	{
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(0.0f, 0.0f)
	};

	
}