#include "LevelVulkanBasics.h"

#include <Engine/vkFramework/VKBuffer.h>

LevelVulkanBasics::LevelVulkanBasics(VulkanInstance& vulkanInstance, VulkanDevice& vulkanDevice)
	: m_vulkanInstance(vulkanInstance), m_vulkanDevice(vulkanDevice)
{
}

bool LevelVulkanBasics::init()
{
	if (!createUniformBuffers()) return false;
	return true;
}

void LevelVulkanBasics::update(double dt)
{
	Level::update(dt);
}

LevelVulkanBasics::~LevelVulkanBasics()
{
	
}

bool LevelVulkanBasics::createUniformBuffers()
{
	VkDeviceSize bufferSize = sizeof(UniformBuffer);

	size_t bufferCount = m_vulkanDevice.swapchainImages.size();

	m_vulkanState.uniformBuffers.resize(bufferCount);
	m_vulkanState.uniformBuffersMemory.resize(bufferCount);

	for (size_t i = 0; i < bufferCount; ++i)
	{
		if (!createBuffer(
			m_vulkanDevice.device, m_vulkanDevice.physicalDevice, bufferSize,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			m_vulkanState.uniformBuffers[i], m_vulkanState.uniformBuffersMemory[i]
		))
		{
			log_error("Failed to create uniform_buffer");
			return false;
		}
	}

	return true;
}

void LevelVulkanBasics::updateUniformBuffer(uint32_t currentImage, const void* uboData, size_t uboSize)
{
	void* data = nullptr;

	vkMapMemory(m_vulkanDevice.device, m_vulkanState.uniformBuffersMemory[currentImage], 0, uboSize, 0, &data);
	memcpy(data, uboData, uboSize);
	vkUnmapMemory(m_vulkanDevice.device, m_vulkanState.uniformBuffersMemory[currentImage]);
}

