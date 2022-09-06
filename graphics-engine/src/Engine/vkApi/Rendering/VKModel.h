#pragma once

#ifdef GRAPHICS_API_VULKAN

#include <Engine/vkApi/VkObjects/VulkanDevice.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

class VKModel
{
public:
	struct Vertex
	{
		glm::vec2 position;
		glm::vec3 color;

		static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
		static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
	};

	VKModel(VulkanDevice &vulkanDevice, const std::vector<Vertex> &verticies);
	~VKModel();

	VKModel(const VKModel &) = delete;
	VKModel &operator=(const VKModel &) = delete;

	void bind(VkCommandBuffer commandBuffer);
	void draw(VkCommandBuffer commandBuffer);

private:
	void createVertexBuffers(const std::vector<Vertex> &verticies);

	VulkanDevice &m_vulkanDevice;
	VkBuffer m_vertexBuffer;
	VkDeviceMemory m_vertexBufferMemory;
	uint32_t m_vertexCount;
};

#endif