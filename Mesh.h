#pragma once

#include "NonCopyable.h"
#include "NonMovable.h"
#include "glm/glm.hpp"
#include "Texture.h"
#include <vector>
#include <string>

struct Vertex
{
	Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& textCoords);
#ifdef DEBUG
	Vertex(const glm::vec3& position);
#endif // DEBUG

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textCoords;
};

class ShaderHandler;
struct Mesh : private NonCopyable
{
	Mesh();
	Mesh(std::vector<Vertex>&& vertices, std::vector<unsigned int>&& indices, 
		std::vector<std::reference_wrapper<const Texture>>&& textures);
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
	std::vector<std::reference_wrapper<const Texture>> textures;
};