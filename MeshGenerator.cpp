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

	constexpr glm::vec3 CUBE_FACE_FRONT_NORMAL = { 0, 0, 1 };
	constexpr std::array<glm::vec3, 4> CUBE_FACE_FRONT =
	{
		glm::vec3(0, 0, CUBE_FACE_SIZE),
		glm::vec3(CUBE_FACE_SIZE, 0, CUBE_FACE_SIZE),
		glm::vec3(CUBE_FACE_SIZE, CUBE_FACE_SIZE, CUBE_FACE_SIZE),
		glm::vec3(0, CUBE_FACE_SIZE, CUBE_FACE_SIZE)
	};

	constexpr glm::vec3 CUBE_FACE_BACK_NORMAL = { 0, 0, -1 };
	constexpr std::array<glm::vec3, 4> CUBE_FACE_BACK =
	{
		glm::vec3(CUBE_FACE_SIZE, 0, 0),
		glm::vec3(0, 0, 0),
		glm::vec3(0, CUBE_FACE_SIZE, 0),
		glm::vec3(CUBE_FACE_SIZE, CUBE_FACE_SIZE, 0)
	};

	constexpr glm::vec3 CUBE_FACE_LEFT_NORMAL = { -1, 0, 0 };
	constexpr std::array<glm::vec3, 4> CUBE_FACE_LEFT =
	{
		glm::vec3(0, 0, 0),
		glm::vec3(0, 0, CUBE_FACE_SIZE),
		glm::vec3(0, CUBE_FACE_SIZE, CUBE_FACE_SIZE),
		glm::vec3(0, CUBE_FACE_SIZE, 0)
	};

	constexpr glm::vec3 CUBE_FACE_RIGHT_NORMAL = { 1, 0, 0 };
	constexpr std::array<glm::vec3, 4> CUBE_FACE_RIGHT =
	{
		glm::vec3(CUBE_FACE_SIZE, 0, CUBE_FACE_SIZE),
		glm::vec3(CUBE_FACE_SIZE, 0, 0),
		glm::vec3(CUBE_FACE_SIZE, CUBE_FACE_SIZE, 0),
		glm::vec3(CUBE_FACE_SIZE, CUBE_FACE_SIZE, CUBE_FACE_SIZE)
	};

	constexpr glm::vec3 CUBE_FACE_TOP_NORMAL = { 0, 1, 0 };
	constexpr std::array<glm::vec3, 4> CUBE_FACE_TOP =
	{
		glm::vec3(0, CUBE_FACE_SIZE, CUBE_FACE_SIZE),
		glm::vec3(CUBE_FACE_SIZE, CUBE_FACE_SIZE, CUBE_FACE_SIZE),
		glm::vec3(CUBE_FACE_SIZE, CUBE_FACE_SIZE, 0),
		glm::vec3(0, CUBE_FACE_SIZE, 0)
	};

	constexpr glm::vec3 CUBE_FACE_BOTTOM_NORMAL = { 0, -1, 0 };
	constexpr std::array<glm::vec3, 4> CUBE_FACE_BOTTOM =
	{
		glm::vec3(0, 0, 0),
		glm::vec3(CUBE_FACE_SIZE, 0, 0),
		glm::vec3(CUBE_FACE_SIZE, 0, CUBE_FACE_SIZE),
		glm::vec3(0, 0, CUBE_FACE_SIZE)
	};
}

void generateCubeFace(eCubeFace cubeFace, Mesh& mesh);

void MeshGenerator::generateCubeMesh(Mesh& mesh)
{
	generateCubeFace(eCubeFace::Left, mesh);
	generateCubeFace(eCubeFace::Right, mesh);
	generateCubeFace(eCubeFace::Top, mesh);
	generateCubeFace(eCubeFace::Bottom, mesh);
	generateCubeFace(eCubeFace::Back, mesh);
	generateCubeFace(eCubeFace::Front, mesh);

	mesh.attachToVAO();
}

void generateCubeFace(eCubeFace cubeFace, Mesh& mesh)
{
	switch (cubeFace)
	{
	case eCubeFace::Left:
	{
		for (const auto& i : CUBE_FACE_LEFT)
		{
			mesh.positions.push_back(i);
			mesh.normals.push_back(CUBE_FACE_LEFT_NORMAL);
		}
	}
	break;
	case eCubeFace::Right:
	{
		for (const auto& i : CUBE_FACE_RIGHT)
		{
			mesh.positions.push_back(i);
			mesh.normals.push_back(CUBE_FACE_RIGHT_NORMAL);
		}
	}
	break;
	case eCubeFace::Top:
	{
		for (const auto& i : CUBE_FACE_TOP)
		{
			mesh.positions.push_back(i);
			mesh.normals.push_back(CUBE_FACE_TOP_NORMAL);
		}
	}
	break;
	case eCubeFace::Bottom:
	{
		for (const auto& i : CUBE_FACE_BOTTOM)
		{
			mesh.positions.push_back(i);
			mesh.normals.push_back(CUBE_FACE_BOTTOM_NORMAL);
		}
	}
	break;
	case eCubeFace::Front:
	{
		for (const auto& i : CUBE_FACE_FRONT)
		{
			mesh.positions.push_back(i);
			mesh.normals.push_back(CUBE_FACE_FRONT_NORMAL);
		}
	}
	break;
	case eCubeFace::Back:
	{
		for (const auto& i : CUBE_FACE_BACK)
		{
			mesh.positions.push_back(i);
			mesh.normals.push_back(CUBE_FACE_BACK_NORMAL);
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