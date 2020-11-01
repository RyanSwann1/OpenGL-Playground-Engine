#pragma once

#include "glm/glm.hpp"
#include <string>
#include <functional>

class ShaderHandler;
struct Model;
class GameObject
{
public:
	GameObject(const Model& model, glm::vec3 startingPosition = glm::vec3(), glm::vec3 startingScale = glm::vec3(1.0f, 1.0f, 1.0f), 
		glm::vec3 startingRotation = glm::vec3());

	void render(ShaderHandler& shaderHandler) const;

private:
	std::reference_wrapper<const Model> m_model;
	glm::vec3 m_position;
	glm::vec3 m_scale;
	glm::vec3 m_rotation;
};