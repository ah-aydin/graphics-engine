#include "kbbpch.h"
#include "OpenGLVertexBuffer.h"

namespace kbb::renderer
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(std::vector<float>& vertices, VertexBufferLayout& layout)
		: VertexBuffer(static_cast<uint32_t>(vertices.size()))
	{
		glCreateBuffers(1, &m_handle);
		glBindBuffer(GL_ARRAY_BUFFER, m_handle);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_vertexCount, vertices.data(), GL_STATIC_DRAW);

		for (int i = 0; i < layout.getCount(); ++i)
		{
			const VertexBufferElement& element = layout.getElement(i);
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, element.count, GL_FLOAT, GL_FALSE, layout.getSize(), (void*)element.offset);
		}
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_handle);
	}

	void OpenGLVertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_handle);
	}

	void OpenGLVertexBuffer::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}