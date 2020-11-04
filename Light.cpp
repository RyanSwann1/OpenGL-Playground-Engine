#include "Light.h"
#ifdef DEBUG
#include "MeshGenerator.h"
#include "glad.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "ShaderHandler.h"
#endif // DEBUG

Light::Light(const glm::vec3& startingPosition, const glm::vec3& startingColor)
	: position(startingPosition),
	color(startingColor)
{
#ifdef DEBUG
	MeshGenerator::generateCubeMesh(mesh);
#endif // DEBUG
}

#ifdef DEBUG
void Light::render(ShaderHandler& shaderHandler) const
{
	mesh.bind();

	glm::mat4 model(1.0f);
	model = glm::translate(model, position);
	shaderHandler.setUniformMat4f(eShaderType::Debug, "uModel", model);

	glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, nullptr);
}
#endif // DEBUG