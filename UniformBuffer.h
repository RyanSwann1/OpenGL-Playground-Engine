#pragma once

#include "NonCopyable.h"
#include "NonMovable.h"
#include "glad.h"
#include "glm/gtc/type_ptr.hpp"
#include <assert.h>
#include <string>

class ShaderHandler;
class UniformBuffer : private NonCopyable, private NonMovable
{
public:
	UniformBuffer(unsigned int bindingPoint, size_t bufferSize, const ShaderHandler& shaderHandler,
		const std::string& uniformBlockName);
	~UniformBuffer();

	void bind() const;

	template <class T>
	void assignBufferSubData(size_t offset, const T& data) const
	{
		assert(offset < m_bufferSize);

		bind();
		glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(T), glm::value_ptr(data));
	}

private:
	unsigned int m_ID;
	unsigned int m_bindingPoint;
	size_t m_bufferSize;
};