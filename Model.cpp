#include "Model.h"
#include "ShaderHandler.h"
#include "ModelLoader.h"
#include "Globals.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

Model::Model(std::vector<Mesh>&& meshes, const std::string& name)
	: name(name),
	meshes(std::move(meshes))
{
	attachMeshesToVAO();
}

void Model::attachMeshesToVAO() const
{
	for (const auto& mesh : meshes)
	{
		mesh.attachToVAO();
	}
}

std::unique_ptr<Model> Model::create(const std::string & fileName)
{
	std::vector<Mesh> meshes;
	if (!ModelLoader::loadModel(fileName, meshes))
	{
		return std::unique_ptr<Model>();
	}

	return std::unique_ptr<Model>(new Model(std::move(meshes), fileName));
}

void Model::render(ShaderHandler& shaderHandler, const glm::vec3& position, glm::vec3 rotation, glm::vec3 scale) const
{	
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::scale(model, scale);
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	
	shaderHandler.setUniformMat4f(eShaderType::Default, "uModel", model);

	for (const auto& mesh : meshes)
	{
		mesh.render(shaderHandler);
	}
}