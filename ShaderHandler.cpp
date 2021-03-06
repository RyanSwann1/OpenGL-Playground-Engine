#include "ShaderHandler.h"
#include "glad.h"
#include "Globals.h"
#include "glm/gtc/type_ptr.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

namespace
{
	const std::string SHADER_DIRECTORY = "Shaders/";
	constexpr int INVALID_UNIFORM_LOCATION = -1;

	bool parseShaderFromFile(const std::string& filePath, std::string& shaderSource)
	{
		std::ifstream stream(filePath);
		if (!stream.is_open())
		{
			return false;
		}
		std::string line;
		std::stringstream stringStream;

		while (getline(stream, line))
		{
			stringStream << line << "\n";
		}

		shaderSource = stringStream.str();
		stream.close();
		stringStream.clear();

		return true;
	}

	bool createShaderProgram(unsigned int shaderID, const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
	{
		std::string vertexShaderSource;
		if (!parseShaderFromFile(SHADER_DIRECTORY + vertexShaderFilePath, vertexShaderSource))
		{
			return false;
		}
		std::string fragmentShaderSource;
		if (!parseShaderFromFile(SHADER_DIRECTORY + fragmentShaderFilePath, fragmentShaderSource))
		{
			return false;
		}

		//Create Vertex Shader
		unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		const char* vertexSrc = vertexShaderSource.c_str();
		glShaderSource(vertexShaderID, 1, &vertexSrc, nullptr);
		glCompileShader(vertexShaderID);

		int vertexShaderResult = 0;
		glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &vertexShaderResult);
		if (vertexShaderResult == GL_FALSE)
		{
			int messageLength = 0;
			glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &messageLength);
			char* errorMessage = static_cast<char*>(alloca(messageLength * sizeof(char)));
			glGetShaderInfoLog(vertexShaderID, messageLength, &messageLength, errorMessage);
			std::cout << "Failed to compile: " << errorMessage << "\n";

			return false;
		}

		//Create Fragment Shader
		unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		const char* fragSrc = fragmentShaderSource.c_str();
		glShaderSource(fragmentShaderID, 1, &fragSrc, nullptr);
		glCompileShader(fragmentShaderID);

		int fragmentShaderResult = 0;
		glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &fragmentShaderResult);
		if (fragmentShaderResult == GL_FALSE)
		{
			int messageLength = 0;
			glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &messageLength);
			char* errorMessage = static_cast<char*>(alloca(messageLength * sizeof(char)));
			glGetShaderInfoLog(fragmentShaderID, messageLength, &messageLength, errorMessage);
			std::cout << "Failed to compile: " << errorMessage << "\n";

			return false;
		}

		glAttachShader(shaderID, vertexShaderID);
		glAttachShader(shaderID, fragmentShaderID);
		glLinkProgram(shaderID);
		glValidateProgram(shaderID);

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);

		return true;
	}
}

//ShaderHandler
ShaderHandler::ShaderHandler()
	: m_currentShaderType(eShaderType::Default)
{}

std::unique_ptr<ShaderHandler> ShaderHandler::create()
{
	std::unique_ptr<ShaderHandler> shaderHandler = std::unique_ptr<ShaderHandler>(new ShaderHandler());
	bool shaderLoaded = false;
	for (const auto& shader : shaderHandler->m_shaders)
	{
		switch (shader.getType())
		{
		case eShaderType::Default:
			shaderLoaded = createShaderProgram(shader.getID(), "VertexShader.glsl", "FragmentShader.glsl");
			break;
#ifdef DEBUG
		case eShaderType::Debug:
			shaderLoaded = createShaderProgram(shader.getID(), "DebugVertexShader.glsl", "DebugFragmentShader.glsl");
			break;
#endif // DEBUG
		default:
			assert(false);
		}
	}

	assert(shaderLoaded);
	return (shaderLoaded ? std::move(shaderHandler) : std::unique_ptr<ShaderHandler>());
}

const ShaderHandler::Shader& ShaderHandler::getShader(eShaderType shaderType) const
{
	return m_shaders[static_cast<int>(m_currentShaderType)];
}

const ShaderHandler::Shader& ShaderHandler::getCurrentActiveShader() const
{
	return m_shaders[static_cast<int>(m_currentShaderType)];
}

void ShaderHandler::setUniformMat3f(eShaderType shaderType, const std::string& uniformName, const glm::mat3& matrix)
{
	assert(shaderType == m_currentShaderType);
	int uniformLocation = m_shaders[static_cast<int>(shaderType)].getUniformLocation(uniformName);
	if (uniformLocation != INVALID_UNIFORM_LOCATION)
	{
		glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
	}
#ifndef DEBUG
	else
	{
		assert(uniformLocation != INVALID_UNIFORM_LOCATION);
	}
#endif // !DEBUG
}

void ShaderHandler::setUniformMat4f(eShaderType shaderType, const std::string& uniformName, const glm::mat4& matrix)
{
	assert(shaderType == m_currentShaderType);
	int uniformLocation = m_shaders[static_cast<int>(shaderType)].getUniformLocation(uniformName);
	if (uniformLocation != INVALID_UNIFORM_LOCATION)
	{
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
	} 
#ifndef DEBUG
	else
	{
		assert(uniformLocation != INVALID_UNIFORM_LOCATION);
	}
#endif // !DEBUG
}

void ShaderHandler::setUniformVec3(eShaderType shaderType, const std::string& uniformName, const glm::vec3& v)
{
	assert(shaderType == m_currentShaderType);
	int uniformLocation = m_shaders[static_cast<int>(shaderType)].getUniformLocation(uniformName);
	if (uniformLocation != INVALID_UNIFORM_LOCATION)
	{
		glUniform3fv(uniformLocation, 1, &v[0]);
	}
#ifndef DEBUG
	else
	{
		assert(uniformLocation != INVALID_UNIFORM_LOCATION);
	}
#endif // !DEBUG
}

void ShaderHandler::setUniformVec4(eShaderType shaderType, const std::string& uniformName, const glm::vec4& v)
{
	assert(shaderType == m_currentShaderType);
	int uniformLocation = m_shaders[static_cast<int>(shaderType)].getUniformLocation(uniformName);
	if (uniformLocation != INVALID_UNIFORM_LOCATION)
	{
		glUniform4fv(uniformLocation, 1, &v[0]);
	}
#ifndef DEBUG
	else
	{
		assert(uniformLocation != INVALID_UNIFORM_LOCATION);
	}
#endif // !DEBUG
}

void ShaderHandler::setUniform1i(eShaderType shaderType, const std::string& uniformName, int value)
{
	assert(shaderType == m_currentShaderType);
	int uniformLocation = m_shaders[static_cast<int>(shaderType)].getUniformLocation(uniformName);
	if (uniformLocation != INVALID_UNIFORM_LOCATION)
	{
		glUniform1i(uniformLocation, value);
	}
#ifndef DEBUG
	else
	{
		assert(uniformLocation != INVALID_UNIFORM_LOCATION);
	}
#endif // !DEBUG
}

void ShaderHandler::setUniform1f(eShaderType shaderType, const std::string& uniformName, float value)
{
	assert(shaderType == m_currentShaderType);
	int uniformLocation = m_shaders[static_cast<int>(shaderType)].getUniformLocation(uniformName);
	if (uniformLocation != INVALID_UNIFORM_LOCATION)
	{
		glUniform1f(uniformLocation, value);
	}
#ifndef DEBUG
	else
	{
		assert(uniformLocation != INVALID_UNIFORM_LOCATION);
	}
#endif // !DEBUG
}

void ShaderHandler::switchToShader(eShaderType shaderType)
{
	m_currentShaderType = shaderType;
	glUseProgram(m_shaders[static_cast<int>(shaderType)].getID());
}

//Shader
ShaderHandler::Shader::Shader(eShaderType shaderType)
	: m_itemID(glCreateProgram()),
	m_type(shaderType),
	m_uniformLocations()
{}

ShaderHandler::Shader::~Shader()
{
	assert(m_itemID != Globals::INVALID_OPENGL_ID);
	glDeleteProgram(m_itemID);
}

unsigned int ShaderHandler::Shader::getID() const
{
	return m_itemID;
}

eShaderType ShaderHandler::Shader::getType() const
{
	return m_type;
}

int ShaderHandler::Shader::getUniformLocation(const std::string& uniformName)
{
	if (m_uniformLocations.find(uniformName) != m_uniformLocations.cend())
	{
		return m_uniformLocations[uniformName];
	}
	else
	{
		int location = glGetUniformLocation(m_itemID, uniformName.c_str());
		if (location >= 0)
		{
			m_uniformLocations[uniformName] = location;
		}

		return location;
	}
}