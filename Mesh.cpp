#include "Mesh.h"

Mesh::Mesh()
	: elementBufferIndex(0),
	vaoID(Globals::INVALID_OPENGL_ID),
	positionsID(Globals::INVALID_OPENGL_ID),
	indiciesID(Globals::INVALID_OPENGL_ID),
	positions(),
	indicies()
{
	glGenVertexArrays(1, &vaoID);
	glGenBuffers(1, &positionsID);
	glGenBuffers(1, &indiciesID);
}

Mesh::~Mesh()
{
	assert(vaoID != Globals::INVALID_OPENGL_ID);
	glDeleteVertexArrays(1, &vaoID);

	assert(postiionsID != Globals::INVALID_OPENGL_ID);
	glDeleteBuffers(1, &positionsID);

	assert(indiciesID != Globals::INVALID_OPENGL_ID);
	glDeleteBuffers(1, &indiciesID);
}

void Mesh::bind() const
{
	glBindVertexArray(vaoID);
}

void Mesh::attachToVAO()
{
	bind();
	
	assert(!positions.empty());
	glBindBuffer(GL_ARRAY_BUFFER, positionsID);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (const void*)0);

	assert(!indicies.empty());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiciesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(unsigned int), indicies.data(), GL_STATIC_DRAW);

	std::vector<glm::vec3> tempPositions;
	positions.swap(tempPositions);

	indicies.shrink_to_fit();

	elementBufferIndex = 0;
}

void Mesh::render() const
{
	bind();
	assert(!indicies.empty());
	glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, nullptr);
}