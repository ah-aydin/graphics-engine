#include "VKModel.h"

#ifdef GRAPHICS_API_VULKAN

#define VK_NO_PROTOTYPES
#include <volk.h>

#include <cassert>

VKModel::VKModel(VulkanDevice& vulkanDevice, const std::vector<Vertex>& verticies)
	: m_vulkanDevice(vulkanDevice)
{
	createVertexBuffers(verticies);
}

VKModel::~VKModel()
{
	vkDestroyBuffer(m_vulkanDevice.device(), m_vertexBuffer, nullptr);
	vkFreeMemory(m_vulkanDevice.device(), m_vertexBufferMemory, nullptr);
}

void VKModel::bind(VkCommandBuffer commandBuffer)
{
	VkBuffer buffers[] = { m_vertexBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
}

void VKModel::draw(VkCommandBuffer commandBuffer)
{
	vkCmdDraw(commandBuffer, m_vertexCount, 1, 0, 0);
}

void VKModel::createVertexBuffers(const std::vector<Vertex>& verticies)
{
	m_vertexCount = static_cast<uint32_t>(verticies.size());
	assert(m_vertexCount >= 3 && "Vertex count must be at least 3");
	VkDeviceSize bufferSize = sizeof(verticies[0]) * m_vertexCount;

	m_vulkanDevice.createBuffer(
		bufferSize,
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
		m_vertexBuffer,
		m_vertexBufferMemory
	);

	void* data;
	vkMapMemory(m_vulkanDevice.device(), m_vertexBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, verticies.data(), static_cast<size_t>(bufferSize));
	vkUnmapMemory(m_vulkanDevice.device(), m_vertexBufferMemory);
}


std::vector<VkVertexInputBindingDescription> VKModel::Vertex::getBindingDescriptions()
{
	std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);

	bindingDescriptions[0].binding = 0;
	bindingDescriptions[0].stride = sizeof(Vertex);
	bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription> VKModel::Vertex::getAttributeDescriptions()
{
	return {
		// { location, binding, format, offset }
		{ 0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, position) },
		{ 1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color) }
	};
}

#endif