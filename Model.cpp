#include "Model.h"
#include "ShaderHandler.h"
#include "ModelLoader.h"
#include "Globals.h"
#include "Transform.h"
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

std::unique_ptr<Model> Model::create(const std::string & fileName, std::vector<std::unique_ptr<Texture>>& loadedTextures)
{
	std::vector<Mesh> meshes;
	if (!ModelLoader::loadModel(fileName, meshes, loadedTextures))
	{
		return std::unique_ptr<Model>();
	}

	return std::unique_ptr<Model>(new Model(std::move(meshes), fileName));
}

void Model::render(ShaderHandler& shaderHandler, const Transform& transform) const
{	
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, transform.position);
	model = glm::scale(model, transform.scale);
	model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	
	shaderHandler.setUniformMat4f(eShaderType::Default, "uModel", model);

	for (const auto& mesh : meshes)
	{
		mesh.render(shaderHandler);
	}
}