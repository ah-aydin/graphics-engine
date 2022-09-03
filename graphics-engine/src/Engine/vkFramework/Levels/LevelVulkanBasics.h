#pragma once

#include <Engine/Level.h>

#include <Engine/vkFramework/VKObjects.h>

#define VK_NO_PROTOTYPES
#include <volk.h>

#include <vector>
#include <glm/glm.hpp>

struct UniformBuffer
{
	glm::mat4 mvp;
};

struct VulkanState
{
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
};

class LevelVulkanBasics : public Level
{
public:
	LevelVulkanBasics(VulkanInstance& vulkanInstance, VulkanDevice& vulkanDevice);
	bool init() override;
	void update(double dt) override;
	~LevelVulkanBasics();

private:

	VulkanInstance m_vulkanInstance;
	VulkanDevice m_vulkanDevice;
	VulkanState m_vulkanState;

	UniformBuffer ubo;
	bool createUniformBuffers();
	void updateUniformBuffer(uint32_t currentImage, const void* uboData, size_t uboSize);
};

