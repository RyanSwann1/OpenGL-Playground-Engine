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
	textures(),
	material(),
	materialMesh(false)
{
	glGenVertexArrays(1, &vaoID);
	glGenBuffers(1, &vboID);
	glGenBuffers(1, &indiciesID);
}

Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<unsigned int>&& indices, 
	std::vector<std::reference_wrapper<const Texture>>&& textures, const Material& material, bool materialMesh)
	: vaoID(Globals::INVALID_OPENGL_ID),
	vboID(Globals::INVALID_OPENGL_ID),
	indiciesID(Globals::INVALID_OPENGL_ID),
	vertices(std::move(vertices)),
	indices(std::move(indices)),
	textures(std::move(textures)),
	material(material),
	materialMesh(materialMesh)
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
	textures(std::move(orig.textures)),
	material(orig.material),
	materialMesh(orig.materialMesh)
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
	material = orig.material;
	materialMesh = orig.materialMesh;

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

	assert(Globals::getTexture(textures, "default_black"));
	glActiveTexture(GL_TEXTURE0);
	Globals::getTexture(textures, "default_black")->bind();
	glActiveTexture(GL_TEXTURE1);
	Globals::getTexture(textures, "default_black")->bind();

	if (materialMesh)
	{
		assert(Globals::getTexture(textures, "default_material"));
		
		glActiveTexture(GL_TEXTURE0);
		Globals::getTexture(textures, "default_material")->bind();
		glActiveTexture(GL_TEXTURE1);
		Globals::getTexture(textures, "default_material")->bind();

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
	}
	else if(!textures.empty())
	{ 
		auto textureDiffuse = std::find_if(textures.cbegin(), textures.cend(), [](const auto& texture)
		{
			return texture.get().type == "texture_diffuse";
		});
		if (textureDiffuse != textures.cend())
		{
			glActiveTexture(GL_TEXTURE0);
			textureDiffuse->get().bind();
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
		}

		auto textureSpecular = std::find_if(textures.cbegin(), textures.cend(), [](const auto& texture)
		{
			return texture.get().type == "texture_specular";
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

//Material
Material::Material()
	: diffuse(),
	specular(),
	ambient(),
	shininess(0.0f)
{}