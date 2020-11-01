#include "GameObject.h"
#include "ModelManager.h"

GameObject::GameObject(const Model& model, glm::vec3 startingPosition, glm::vec3 startingScale, glm::vec3 startingRotation)
	: m_model(model),
	m_position(startingPosition),
	m_scale(startingScale),
	m_rotation(startingRotation)
{}

void GameObject::render(ShaderHandler& shaderHandler) const
{
	m_model.get().render(shaderHandler, m_position, m_rotation, m_scale);
}