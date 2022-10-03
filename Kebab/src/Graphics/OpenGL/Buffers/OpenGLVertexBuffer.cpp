#include "kbbpch.h"
#include "OpenGLVertexBuffer.h"

namespace kbb::renderer
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(std::vector<float>& vertices, VertexBufferLayout& layout)
		: VertexBuffer(static_cast<uint32_t>(vertices.size()))
	{
		KBB_GL_CALL(glCreateBuffers(1, &m_handle));
		KBB_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_handle));
		KBB_GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_vertexCount, vertices.data(), GL_STATIC_DRAW));

		for (int i = 0; i < layout.getCount(); ++i)
		{
			const VertexBufferElement& element = layout.getElement(i);
			KBB_GL_CALL(glEnableVertexAttribArray(i));
			KBB_GL_CALL(glVertexAttribPointer(i, element.count, GL_FLOAT, GL_FALSE, layout.getSize(), (void*)element.offset));
		}
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		KBB_GL_CALL(glDeleteBuffers(1, &m_handle));
	}

	void OpenGLVertexBuffer::bind() const
	{
		KBB_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_handle));
	}

	void OpenGLVertexBuffer::unbind() const
	{
		KBB_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
}