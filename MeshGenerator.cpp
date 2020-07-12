#include "MeshGenerator.h"
#include "Mesh.h"
#include <array>

namespace
{
	constexpr unsigned int CUBE_FACE_INDICIE_COUNT = 4;

	constexpr std::array<unsigned int, 6> CUBE_FACE_INDICIES =
	{
		0, 1, 2,
		2, 3, 0
	};

	enum class eCubeFace
	{
		Left,
		Right,
		Front,
		Back,
		Top,
		Bottom
	};

	constexpr float CUBE_FACE_SIZE = 1.0f;

	//Cube Position Coords
	constexpr std::array<glm::vec3, 4> CUBE_FACE_FRONT =
	{
		glm::vec3(0, 0, CUBE_FACE_SIZE),
		glm::vec3(CUBE_FACE_SIZE, 0, CUBE_FACE_SIZE),
		glm::vec3(CUBE_FACE_SIZE, CUBE_FACE_SIZE, CUBE_FACE_SIZE),
		glm::vec3(0, CUBE_FACE_SIZE, CUBE_FACE_SIZE)
	};

	constexpr std::array<glm::vec3, 4> CUBE_FACE_BACK =
	{
		glm::vec3(CUBE_FACE_SIZE, 0, 0),
		glm::vec3(0, 0, 0),
		glm::vec3(0, CUBE_FACE_SIZE, 0),
		glm::vec3(CUBE_FACE_SIZE, CUBE_FACE_SIZE, 0)
	};

	constexpr std::array<glm::vec3, 4> CUBE_FACE_LEFT =
	{
		glm::vec3(0, 0, 0),
		glm::vec3(0, 0, CUBE_FACE_SIZE),
		glm::vec3(0, CUBE_FACE_SIZE, CUBE_FACE_SIZE),
		glm::vec3(0, CUBE_FACE_SIZE, 0)
	};

	constexpr std::array<glm::vec3, 4> CUBE_FACE_RIGHT =
	{
		glm::vec3(CUBE_FACE_SIZE, 0, CUBE_FACE_SIZE),
		glm::vec3(CUBE_FACE_SIZE, 0, 0),
		glm::vec3(CUBE_FACE_SIZE, CUBE_FACE_SIZE, 0),
		glm::vec3(CUBE_FACE_SIZE, CUBE_FACE_SIZE, CUBE_FACE_SIZE)
	};

	constexpr std::array<glm::vec3, 4> CUBE_FACE_TOP =
	{
		glm::vec3(0, CUBE_FACE_SIZE, CUBE_FACE_SIZE),
		glm::vec3(CUBE_FACE_SIZE, CUBE_FACE_SIZE, CUBE_FACE_SIZE),
		glm::vec3(CUBE_FACE_SIZE, CUBE_FACE_SIZE, 0),
		glm::vec3(0, CUBE_FACE_SIZE, 0)
	};

	constexpr std::array<glm::vec3, 4> CUBE_FACE_BOTTOM =
	{
		glm::vec3(0, 0, 0),
		glm::vec3(CUBE_FACE_SIZE, 0, 0),
		glm::vec3(CUBE_FACE_SIZE, 0, CUBE_FACE_SIZE),
		glm::vec3(0, 0, CUBE_FACE_SIZE)
	};
}

void generateCubeFace(const glm::vec3& position, eCubeFace cubeFace, Mesh& mesh);

void MeshGenerator::generateCubeMesh(const glm::vec3& position, Mesh& mesh)
{
	generateCubeFace(position, eCubeFace::Left, mesh);
	generateCubeFace(position, eCubeFace::Right, mesh);
	generateCubeFace(position, eCubeFace::Top, mesh);
	generateCubeFace(position, eCubeFace::Bottom, mesh);
	generateCubeFace(position, eCubeFace::Back, mesh);
	generateCubeFace(position, eCubeFace::Front, mesh);

	mesh.attachToVAO();
}

void generateCubeFace(const glm::vec3& position, eCubeFace cubeFace, Mesh& mesh)
{
	switch (cubeFace)
	{
	case eCubeFace::Left:
	{
		for (const auto& i : CUBE_FACE_LEFT)
		{
			mesh.positions.emplace_back(i + position);
		}
	}
	break;
	case eCubeFace::Right:
	{
		for (const auto& i : CUBE_FACE_RIGHT)
		{
			mesh.positions.emplace_back(i + position);
		}
	}
	break;
	case eCubeFace::Top:
	{
		for (const auto& i : CUBE_FACE_TOP)
		{
			mesh.positions.emplace_back(i + position);
		}
	}
	break;
	case eCubeFace::Bottom:
	{
		for (const auto& i : CUBE_FACE_BOTTOM)
		{
			mesh.positions.emplace_back(i + position);
		}
	}
	break;
	case eCubeFace::Front:
	{
		for (const auto& i : CUBE_FACE_FRONT)
		{
			mesh.positions.emplace_back(i + position);
		}
	}
	break;
	case eCubeFace::Back:
	{
		for (const auto& i : CUBE_FACE_BACK)
		{
			mesh.positions.emplace_back(i + position);
		}
	}
	break;
	}

	for (unsigned int i : CUBE_FACE_INDICIES)
	{
		mesh.indicies.emplace_back(i + mesh.elementBufferIndex);
	}

	mesh.elementBufferIndex += CUBE_FACE_INDICIE_COUNT;
}