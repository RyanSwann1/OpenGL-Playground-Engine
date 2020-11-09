#include "UniformBuffer.h"
#include "ShaderHandler.h"
#include "Globals.h"

UniformBuffer::UniformBuffer(unsigned int bindingPoint, size_t bufferSize, const ShaderHandler& shaderHandler,
	const std::string& uniformBlockName)
	: m_ID(Globals::INVALID_OPENGL_ID),
	m_bindingPoint(bindingPoint),
	m_bufferSize(bufferSize)
{
	glGenBuffers(1, &m_ID);
	
	bind();	
	glBufferData(GL_UNIFORM_BUFFER, bufferSize, nullptr, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, m_bindingPoint, m_ID, 0, bufferSize);
	unsigned int shaderID = shaderHandler.getCurrentActiveShader().getID();
	glUniformBlockBinding(shaderID, glGetUniformBlockIndex(shaderID, uniformBlockName.data()), m_bindingPoint);
}

UniformBuffer::~UniformBuffer()
{
	glDeleteBuffers(1, &m_ID);
}

void UniformBuffer::bind() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
}