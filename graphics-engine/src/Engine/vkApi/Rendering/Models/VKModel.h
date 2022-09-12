#pragma once

#ifdef GRAPHICS_API_VULKAN

#include <Engine/vkApi/VkObjects/VulkanDevice.h>

#define VK_NO_PROTOTYPES
#include <volk.h>

#include <cassert>
#include <vector>

template <typename VertexType>
class VKModel
{
protected:
	VKModel(VulkanDevice& vulkanDevice, const std::vector<VertexType>& verticies)
		: m_vulkanDevice(vulkanDevice)
	{
		createVertexBuffers(verticies);
	}

	virtual ~VKModel()
	{
		vkDestroyBuffer(m_vulkanDevice.device(), m_vertexBuffer, nullptr);
		vkFreeMemory(m_vulkanDevice.device(), m_vertexBufferMemory, nullptr);
	};

	VKModel(const VKModel&) = delete;
	VKModel& operator=(const VKModel&) = delete;

protected:
	void bind(VkCommandBuffer commandBuffer)
	{
		VkBuffer buffers[] = { m_vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
	}

	void draw(VkCommandBuffer commandBuffer) { vkCmdDraw(commandBuffer, m_vertexCount, 1, 0, 0); }

protected:
	void createVertexBuffers(const std::vector<VertexType>& verticies)
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

	VulkanDevice& m_vulkanDevice;
	VkBuffer m_vertexBuffer;
	VkDeviceMemory m_vertexBufferMemory;
	uint32_t m_vertexCount;
};

#endif