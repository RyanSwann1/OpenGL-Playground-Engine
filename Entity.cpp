#include "Entity.h"
#include "ModelManager.h"

Entity::Entity(const Model& model, glm::vec3 startingPosition, glm::vec3 startingScale, glm::vec3 startingRotation)
	: m_model(model),
	m_position(startingPosition),
	m_scale(startingScale),
	m_rotation(startingRotation)
{}

void Entity::render(ShaderHandler& shaderHandler) const
{
	m_model.get().render(shaderHandler, m_position, m_rotation, m_scale);
}