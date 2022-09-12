#pragma once

#ifdef GRAPHICS_API_VULKAN

#define VK_NO_PROTOTYPES

#include <Engine/vkApi/VkObjects/VulkanDevice.h>
#include <Engine/vkApi/VkObjects/VulkanPipeline.h>
#include <Engine/vkApi/GameObject/VKGameObject2D.h>

#include <vector>
#include <memory>

class BasicRenderSystem2D
{
public:
	BasicRenderSystem2D(VulkanDevice &device, VkRenderPass renderPass);
	~BasicRenderSystem2D();

	BasicRenderSystem2D(const BasicRenderSystem2D&) = delete;
	BasicRenderSystem2D& operator=(const BasicRenderSystem2D) = delete;

	void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<VKGameObject2D> &gameObjects);
private:

	VulkanDevice &m_vulkanDevice;

	void createPipelineLayout();
	void createPipeline(VkRenderPass renderPass);
	std::unique_ptr<VulkanPipeline> m_vulkanPipeline;
	VkPipelineLayout m_pipelineLayout;
};

#endif