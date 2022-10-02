#include "kbbpch.h"
#include "VulkanVertexBuffer.h"

namespace kbb::renderer
{
	VulkanVertexBuffer::VulkanVertexBuffer(std::vector<float>& vertices)
		: VertexBuffer(static_cast<uint32_t>(vertices.size()))
	{
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
	}

	void VulkanVertexBuffer::bind() const
	{
	}

	void VulkanVertexBuffer::unbind() const
	{
	}
}
