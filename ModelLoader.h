#pragma once

#include <string>
#include <vector>
#include <memory>

struct Texture;
struct Mesh;
namespace ModelLoader
{
	bool loadModel(const std::string& fileName, std::vector<Mesh>& meshes, std::vector<std::unique_ptr<Texture>>& loadedTextures);
}