#pragma once

#include "Globals.h"
#include "NonCopyable.h"
#include "NonMovable.h"
#include "glm/glm.hpp"
#include "glad.h"
#include <vector>

struct Mesh final : private NonCopyable, private NonMovable
{
	Mesh();
	~Mesh();

	void bind() const;
	void attachToVAO();
	void render() const;

	int elementBufferIndex;
	unsigned int vaoID;
	unsigned int positionsID;
	unsigned int normalsID;
	unsigned int indiciesID;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indicies;
};