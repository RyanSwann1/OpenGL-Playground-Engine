#include "Light.h"
#include "ShaderHandler.h"
#ifdef DEBUG
#include "MeshGenerator.h"
#include "glad.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#endif // DEBUG

//DirectionalLight
DirectionalLight::DirectionalLight(ShaderHandler& shaderHandler, const glm::vec3& direction, const glm::vec3& color,
	float intensity)
	: direction(direction),
	color(color),
	intensity(intensity)
{
	shaderHandler.setUniformVec3(eShaderType::Default, "uDirectionalLight", direction);
	shaderHandler.setUniformVec3(eShaderType::Default, "uDirectionalLightColor", color);
	shaderHandler.setUniform1f(eShaderType::Default, "uDirectionalLightIntensity", intensity);
}

//Light
Light::Light(const glm::vec3& startingPosition, const glm::vec3& startingColor, float radius, float compression)
	: position(glm::vec4(startingPosition, 1.0f)),
	color(glm::vec4(startingColor, 1.0)),
	radius(radius),
	compression(compression)
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
	model = glm::translate(model, glm::vec3(position));
	shaderHandler.setUniformMat4f(eShaderType::Debug, "uModel", model);

	glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, nullptr);
}
#endif // DEBUG