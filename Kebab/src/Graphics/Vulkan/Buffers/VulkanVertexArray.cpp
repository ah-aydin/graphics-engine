#include "kbbpch.h"
#include "VulkanVertexArray.h"

namespace kbb::renderer
{
	VulkanVertexArray::VulkanVertexArray(std::vector<float>& vertices, std::vector<uint32_t> indices)
	{
	}

	VulkanVertexArray::~VulkanVertexArray()
	{
	}

	void VulkanVertexArray::bind() const
	{
	}

	void VulkanVertexArray::unbind() const
	{
	}

	uint32_t VulkanVertexArray::getVertexCount() const
	{
		return m_vertexBuffer->getVertexCount();
	}

	uint32_t VulkanVertexArray::getIndexCount() const
	{
		return m_indexBuffer->getIndexCount();
	}
}
