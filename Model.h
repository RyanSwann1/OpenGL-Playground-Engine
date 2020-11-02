#pragma once

#include "NonCopyable.h"
#include "NonMovable.h"
#include "Mesh.h"
#include <string>
#include <memory>
#include <vector>

struct Transform;
class ShaderHandler;
struct Model : private NonMovable, private NonCopyable
{
	static std::unique_ptr<Model> create(const std::string& fileName);

	void render(ShaderHandler& shaderHandler, const Transform& transform) const;

	const std::string name;
	const std::vector<Mesh> meshes;
	
private:
	Model(std::vector<Mesh>&& meshes, const std::string& name);

	void attachMeshesToVAO() const;
};