#pragma once

#define VK_NO_PROTOTYPES
#include <volk.h>
#include <glm/glm.hpp>

struct VulkanDevice;

struct UniformBuffer
{
	glm::mat4 mvp;
};

bool createBuffer(
	VkDevice device,
	VkPhysicalDevice physicalDevice,
	VkDeviceSize size,
	VkBufferUsageFlags usage,
	VkMemoryPropertyFlags properties,
	VkBuffer& buffer,
	VkDeviceMemory& bufferMemory
);

void copyBuffer(VulkanDevice& vulkanDevice, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

bool createUniformBuffer(VulkanDevice& vulkanDevice, VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkDeviceSize size, const void* data, const size_t dataSize);