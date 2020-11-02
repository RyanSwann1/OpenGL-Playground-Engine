#include "Light.h"
#ifdef DEBUG_LIGHTING
#include "MeshGenerator.h"
#include "glad.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "ShaderHandler.h"
#endif // DEBUG_LIGHTING

Light::Light(const glm::vec3& startingPosition, const glm::vec3& startingColor)
	: position(startingPosition),
	color(startingColor)
{}

#ifdef DEBUG_LIGHTING
void Light::render(ShaderHandler& shaderHandler) 
{
	mesh.vertices.clear();
	mesh.indices.clear();

	MeshGenerator::generateCubeMesh(mesh, position);

	glm::mat4 model(1.0f);
	model = glm::translate(model, position);
	shaderHandler.setUniformMat4f(eShaderType::Debug, "uModel", model);

	glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, nullptr);
}
#endif // DEBUG_LIGHTING