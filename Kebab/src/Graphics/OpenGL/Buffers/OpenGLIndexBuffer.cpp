#include "kbbpch.h"
#include "OpenGLIndexBuffer.h"

namespace kbb::renderer
{
	OpenGLIndexBuffer::OpenGLIndexBuffer(std::vector<uint32_t>& indices)
		: IndexBuffer(static_cast<uint32_t>(indices.size()))
	{
		KBB_GL_CALL(glCreateBuffers(1, &m_buffer));
		KBB_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer));
			KBB_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW));
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		KBB_GL_CALL(glDeleteBuffers(1, &m_buffer));
	}

	void OpenGLIndexBuffer::bind() const
	{
		KBB_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer));
	}

	void OpenGLIndexBuffer::unbind() const
	{
		KBB_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
}