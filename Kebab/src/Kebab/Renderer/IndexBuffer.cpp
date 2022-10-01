#include "kbbpch.h"
#include "IndexBuffer.h"

namespace kbb
{
	IndexBuffer* IndexBuffer::create(std::vector<uint32_t> indices)
	{
		// TODO create index buffer for the given rendering api
		return nullptr;
	}

	IndexBuffer::IndexBuffer(size_t indexCount)
		: m_indexCount(static_cast<uint32_t>(indexCount))
	{}
}
