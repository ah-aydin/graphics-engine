#include "kbbpch.h"
#include "OpenGLVertexArray.h"

namespace kbb::renderer
{
	OpenGLVertexArray::OpenGLVertexArray(std::vector<float>& vertices, VertexBufferLayout& layout, std::vector<uint32_t>& indices)
		: VertexArray()
	{
		glCreateVertexArrays(1, &m_handle);
		glBindVertexArray(m_handle);

		m_vertexBuffer = std::make_shared<OpenGLVertexBuffer>(vertices, layout);
		m_indexBuffer = std::make_shared<OpenGLIndexBuffer>(indices);

		glBindVertexArray(0);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_handle);
	}

	void OpenGLVertexArray::bind() const
	{
		glBindVertexArray(m_handle);
	}

	void OpenGLVertexArray::unbind() const
	{
		glBindVertexArray(0);
	}

	uint32_t OpenGLVertexArray::getVertexCount() const
	{
		return m_vertexBuffer->getVertexCount();
	}

	uint32_t OpenGLVertexArray::getIndexCount() const
	{
		return m_indexBuffer->getIndexCount();
	}
}