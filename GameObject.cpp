#include "GameObject.h"
#include "ModelManager.h"

GameObject::GameObject(const Model& model, const Transform& transform)
	: m_model(model),
	m_transform(transform)
{}

void GameObject::render(ShaderHandler& shaderHandler, const glm::mat4& view) const
{
	m_model.get().render(shaderHandler, m_transform, view);
}