#include "Mesh.h"
#include "Globals.h"
#include "glad.h"
#include "ShaderHandler.h"
#include <iostream>

//Mesh
Mesh::Mesh()
	: vaoID(Globals::INVALID_OPENGL_ID),
	vboID(Globals::INVALID_OPENGL_ID),
	indiciesID(Globals::INVALID_OPENGL_ID),
	vertices(),
	indices(),
	textures()
{
	glGenVertexArrays(1, &vaoID);
	glGenBuffers(1, &vboID);
	glGenBuffers(1, &indiciesID);
}

Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<unsigned int>&& indices, 
	std::vector<std::reference_wrapper<const Texture>>&& textures)
	: vaoID(Globals::INVALID_OPENGL_ID),
	vboID(Globals::INVALID_OPENGL_ID),
	indiciesID(Globals::INVALID_OPENGL_ID),
	vertices(std::move(vertices)),
	indices(std::move(indices)),
	textures(std::move(textures))
{
	glGenVertexArrays(1, &vaoID);
	glGenBuffers(1, &vboID);
	glGenBuffers(1, &indiciesID);
}

Mesh::Mesh(Mesh&& orig) noexcept
	: vaoID(orig.vaoID),
	vboID(orig.vboID),
	indiciesID(orig.indiciesID),
	vertices(std::move(orig.vertices)),
	indices(std::move(orig.indices)),
	textures(std::move(orig.textures))
{
	orig.vaoID = Globals::INVALID_OPENGL_ID;
	orig.vboID = Globals::INVALID_OPENGL_ID;
	orig.indiciesID = Globals::INVALID_OPENGL_ID;
}

Mesh& Mesh::operator=(Mesh&& orig) noexcept
{
	vaoID = orig.vaoID;
	vboID = orig.vboID;
	indiciesID = orig.indiciesID;
	vertices = std::move(orig.vertices);
	indices = std::move(orig.indices);
	textures = std::move(orig.textures);

	orig.vaoID = Globals::INVALID_OPENGL_ID;
	orig.vboID = Globals::INVALID_OPENGL_ID;
	orig.indiciesID = Globals::INVALID_OPENGL_ID;

	return *this;
}

Mesh::~Mesh()
{
	if (vaoID != Globals::INVALID_OPENGL_ID &&
		vboID != Globals::INVALID_OPENGL_ID &&
		indiciesID != Globals::INVALID_OPENGL_ID)
	{
		glDeleteVertexArrays(1, &vaoID);
		glDeleteBuffers(1, &vboID);
		glDeleteBuffers(1, &indiciesID);
	}
	else
	{
		assert(vaoID == Globals::INVALID_OPENGL_ID &&
			vboID == Globals::INVALID_OPENGL_ID &&
			indiciesID == Globals::INVALID_OPENGL_ID);
	}
}

void Mesh::bind() const
{
    glBindVertexArray(vaoID);
}

void Mesh::attachToVAO() const
{
    bind();

	assert(!vertices.empty());
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, textCoords));

	assert(!indices.empty());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiciesID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
}

void Mesh::render(ShaderHandler& shaderHandler) const
{
	bind();

	assert(Globals::getTexture(textures, Globals::DEFAULT_BLACK));
	glActiveTexture(GL_TEXTURE0);
	Globals::getTexture(textures, Globals::DEFAULT_BLACK)->bind();
	glActiveTexture(GL_TEXTURE1);
	Globals::getTexture(textures, Globals::DEFAULT_BLACK)->bind();

	if (!Globals::getTexture(textures, Globals::TEXTURE_DIFFUSE))
	{
		assert(Globals::getTexture(textures, Globals::DEFAULT_MATERIAL));

		glActiveTexture(GL_TEXTURE0);
		Globals::getTexture(textures, Globals::DEFAULT_MATERIAL)->bind();
		glActiveTexture(GL_TEXTURE1);
		Globals::getTexture(textures, Globals::DEFAULT_MATERIAL)->bind();

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
	}
	else
	{
		auto textureDiffuse = std::find_if(textures.cbegin(), textures.cend(), [](const auto& texture)
		{
			return texture.get().type == Globals::TEXTURE_DIFFUSE;
		});
		if (textureDiffuse != textures.cend())
		{
			glActiveTexture(GL_TEXTURE0);
			textureDiffuse->get().bind();
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
		}

		auto textureSpecular = std::find_if(textures.cbegin(), textures.cend(), [](const auto& texture)
		{
			return texture.get().type == Globals::TEXTURE_SPECULAR;
		});
		if (textureSpecular != textures.cend())
		{
			glActiveTexture(GL_TEXTURE1);
			textureSpecular->get().bind();
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
		}
	}
}

//Vertex
Vertex::Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& textCoords)
	: position(position),
	normal(normal),
	textCoords(textCoords)
{}

#ifdef DEBUG
Vertex::Vertex(const glm::vec3& position)
	: position(position),
	normal(),
	textCoords()
{}
#endif // DEBUG