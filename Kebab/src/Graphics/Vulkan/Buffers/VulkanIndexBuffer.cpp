#include "kbbpch.h"
#include "VulkanIndexBuffer.h"

namespace kbb::renderer
{
	VulkanIndexBuffer::VulkanIndexBuffer(std::vector<uint32_t>& indices)
		: IndexBuffer(static_cast<uint32_t>(indices.size()))
	{
	}

	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
	}

	void VulkanIndexBuffer::bind() const
	{
	}

	void VulkanIndexBuffer::unbind() const
	{
	}
}
