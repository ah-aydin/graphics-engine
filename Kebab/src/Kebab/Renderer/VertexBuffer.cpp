#include "kbbpch.h"
#include "VertexBuffer.h"

namespace kbb
{
	VertexBuffer* VertexBuffer::create(std::vector<float> vertices, std::optional<VertexBufferLayout> layout)
	{
		// TODO create vertex buffer for the given api
		return nullptr;
	}

	VertexBufferLayout::VertexBufferLayout(std::initializer_list<VertexBufferElement> elements)
		: m_elements(elements)
	{
		uint32_t offset = 0;
		m_stride = 0;
		for (VertexBufferElement& element : m_elements)
		{
			element.offset = offset;
			offset += element.getSize();
			m_stride += element.getSize();
		}
	}
}