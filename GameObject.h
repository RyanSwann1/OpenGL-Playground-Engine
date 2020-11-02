#pragma once

#include "Transform.h"
#include <functional>

class ShaderHandler;
struct Model;
class GameObject
{
public:
	GameObject(const Model& model, const Transform& transform);

	void render(ShaderHandler& shaderHandler) const;

private:
	std::reference_wrapper<const Model> m_model;
	Transform m_transform;
};