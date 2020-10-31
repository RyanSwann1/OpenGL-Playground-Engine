#pragma once

#include "NonCopyable.h"
#include "NonMovable.h"
#include "Mesh.h"
#include <string>
#include <memory>
#include <vector>

class ShaderHandler;
struct Model : private NonMovable, private NonCopyable
{
	static std::unique_ptr<Model> create(const std::string& fileName);

	void render(ShaderHandler& shaderHandler, const glm::vec3& position, glm::vec3 rotation = glm::vec3(), 
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)) const;

	const std::string name;
	const std::vector<Mesh> meshes;
	
private:
	Model(std::vector<Mesh>&& meshes, const std::string& name);

	void attachMeshesToVAO() const;
};