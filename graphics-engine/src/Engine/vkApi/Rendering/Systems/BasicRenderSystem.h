#pragma once

#ifdef GRAPHICS_API_VULKAN

#define VK_NO_PROTOTYPES

#include <Engine/vkApi/VkObjects/VulkanDevice.h>
#include <Engine/vkApi/VkObjects/VulkanPipeline.h>
#include <Engine/vkApi/GameObject/VKGameObject2D.h>

#include <vector>
#include <memory>

class BasicRenderSystem
{
public:
	BasicRenderSystem(VulkanDevice &device, VkRenderPass renderPass);
	~BasicRenderSystem();

	BasicRenderSystem(const BasicRenderSystem&) = delete;
	BasicRenderSystem& operator=(const BasicRenderSystem) = delete;

	void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<VKGameObject2D> &gameObjects);
private:

	VulkanDevice &m_vulkanDevice;

	void createPipelineLayout();
	void createPipeline(VkRenderPass renderPass);
	std::unique_ptr<VulkanPipeline> m_vulkanPipeline;
	VkPipelineLayout m_pipelineLayout;
};

#endif