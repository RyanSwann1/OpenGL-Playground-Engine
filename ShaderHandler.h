#pragma once

#include "NonCopyable.h"
#include "NonMovable.h"
#include "glm/glm.hpp"
#include <memory>
#include <array>
#include <vector>
#include <unordered_map>
#include <string>

enum class eShaderType
{
	Default = 0,
#ifdef DEBUG
	Debug,
	Max = Debug
#endif // DEBUG
#ifndef DEBUG
	Max = Default
#endif // !DEBUG
};

class ShaderHandler final : private NonCopyable, private NonMovable
{
	class Shader : private NonCopyable
	{
	public:
		Shader(eShaderType shaderType);
		~Shader();

		unsigned int getID() const;
		eShaderType getType() const;
		int getUniformLocation(const std::string& uniformName);

	private:
		unsigned int m_itemID;
		eShaderType m_type;
		std::unordered_map<std::string, int> m_uniformLocations;
	};

public:
	static std::unique_ptr<ShaderHandler> create();

	const Shader& getShader(eShaderType shaderType) const;
	const Shader& getCurrentActiveShader() const;

	void setUniformMat3f(eShaderType shaderType, const std::string& uniformName, const glm::mat3& matrix);
	void setUniformMat4f(eShaderType shaderType, const std::string& uniformName, const glm::mat4& matrix);
	void setUniformVec3(eShaderType shaderType, const std::string& uniformName, const glm::vec3& v);
	void setUniformVec4(eShaderType shaderType, const std::string& uniformName, const glm::vec4& v);
	void setUniform1i(eShaderType shaderType, const std::string& uniformName, int value);
	void setUniform1f(eShaderType shaderType, const std::string& uniformName, float value);
	void switchToShader(eShaderType shaderType);

private:
	ShaderHandler();
	eShaderType m_currentShaderType;

	std::array<Shader, static_cast<int>(eShaderType::Max) + 1> m_shaders =
	{
#ifdef DEBUG
		eShaderType::Default,
		eShaderType::Debug
#endif // DEBUG
#ifndef DEBUG
		eShaderType::Default
#endif // !DEBUG
	};
};