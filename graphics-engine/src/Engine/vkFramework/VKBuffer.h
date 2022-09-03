#pragma once

#define VK_NO_PROTOTYPES
#include <volk.h>
#include <glm/glm.hpp>

struct VulkanTexture
{
	VkImage image;
	VkDeviceMemory imageMemory;
	VkImageView imageView;
};

struct VulkanDevice;

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

bool createImage(
	VkDevice device,
	VkPhysicalDevice physicalDevice,
	uint32_t width,
	uint32_t height,
	VkFormat format,
	VkImageTiling tiling,
	VkImageUsageFlags usage,
	VkMemoryPropertyFlags properties,
	VkImage& image,
	VkDeviceMemory& imageMemory
);

bool createTextureSampler(VkDevice device, VkSampler* sampler);
void copyBufferToImage(VulkanDevice& vulkanDevice, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
void destroyVulkanTexture(VkDevice device, VulkanTexture& texture);
void transitionImageLayout(
	VulkanDevice& vulkanDevice,
	VkImage image,
	VkFormat format,
	VkImageLayout oldLayout,
	VkImageLayout newLayout,
	uint32_t layerCount,
	uint32_t mipLevels
);