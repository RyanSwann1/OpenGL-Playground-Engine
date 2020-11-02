#pragma once

#include "NonCopyable.h"
#include "NonMovable.h"
#include "glm/glm.hpp"
#include <vector>
#include <string>

//https://vulkan-tutorial.com/Loading_models

struct Material 
{
	Material();

	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 ambient;
	float shininess;
};

struct MeshTextureDetails : private NonCopyable
{
	MeshTextureDetails(unsigned int ID, const std::string& type, const std::string& path);
	MeshTextureDetails(MeshTextureDetails&&) noexcept;
	MeshTextureDetails& operator=(MeshTextureDetails&&) noexcept;
	~MeshTextureDetails();

	unsigned int ID;
	std::string type;
	std::string path;
};

struct Vertex
{
	Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& textCoords);
#ifdef DEBUG_LIGHTING
	Vertex(const glm::vec3& position);
#endif // DEBUG_LIGHTING

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textCoords;
};

class ShaderHandler;
struct Mesh : private NonCopyable
{
	Mesh();
	Mesh(std::vector<Vertex>&& vertices, std::vector<unsigned int>&& indices, std::vector<MeshTextureDetails>&& textures, const Material& material);
	Mesh(Mesh&&) noexcept;
	Mesh& operator=(Mesh&&) noexcept;
	~Mesh();

	void bind() const;
	void attachToVAO() const;
	void render(ShaderHandler& shaderHandler) const;

	unsigned int vaoID;
	unsigned int vboID;
	unsigned int indiciesID;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<MeshTextureDetails> textures;
	Material material;
};