#include "kbbpch.h"
#include "VertexBuffer.h"

#ifdef GRAPHICS_API_OPENGL
#include <Graphics/OpenGL/Buffers/OpenGLVertexBuffer.h>
#endif

namespace kbb::renderer
{
	std::shared_ptr<VertexBuffer> VertexBuffer::Create(std::vector<float>& vertices, VertexBufferLayout& layout)
	{
#ifdef GRAPHICS_API_OPENGL
		return std::make_shared<OpenGLVertexBuffer>(vertices, layout);
#endif
	}

	VertexBuffer::VertexBuffer(uint32_t vertexCount)
		: m_vertexCount(vertexCount)
	{}

	VertexBufferLayout::VertexBufferLayout(std::vector<VertexBufferElement> elements)
		: m_elements{elements}, m_size(0)
	{
		for (VertexBufferElement& element : m_elements)
		{
			element.offset = m_size;
			m_size += element.size;
		}
	}

	const VertexBufferElement& VertexBufferLayout::getElement(size_t index) const
	{
		return m_elements.at(index);
	}
}
