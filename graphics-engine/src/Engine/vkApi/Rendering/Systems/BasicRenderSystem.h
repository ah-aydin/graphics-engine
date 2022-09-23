#pragma once

#ifdef GRAPHICS_API_VULKAN

#define VK_NO_PROTOTYPES

#include <Engine/vkApi/VkObjects/VulkanDevice.h>
#include <Engine/vkApi/VkObjects/VulkanPipeline.h>
#include <Engine/vkApi/GameObject/VKGameObject2D.h>
#include <Engine/vkApi/GameObject/VKGameObject3D.h>
#include <Engine/vkApi/Rendering/RenderDimention.h>

#include <vector>
#include <memory>

class BasicRenderSystem
{
public:
	BasicRenderSystem(
		VulkanDevice &device,
		VkRenderPass renderPass,
		std::string vertFilePath,
		std::string fragFilePath,
		const std::vector<VkBuffer>& uniformBuffers,
		RenderDimention renderDimention = RenderDimention::RENDER3D
	);
	~BasicRenderSystem();

	BasicRenderSystem(const BasicRenderSystem&) = delete;
	BasicRenderSystem& operator=(const BasicRenderSystem) = delete;

	void renderGameObjects2D(VkCommandBuffer commandBuffer, std::vector<VKGameObject2D> &gameObjects);
	void renderGameObjects3D(
		VkCommandBuffer commandBuffer,
		VkDeviceMemory uniformBufferMemory,
		int currentImageIndex,
		std::vector<VKGameObject3D>& gameObjects
	);

private:
	RenderDimention m_renderDimention;

	VulkanDevice &m_vulkanDevice;

	void createPipelineLayout();
	void createPipeline(
		VkRenderPass renderPass,
		RenderDimention renderDimention,
		std::string vertFilePath,
		std::string fragFilePath
	);
	std::unique_ptr<VulkanPipeline> m_vulkanPipeline;
	VkPipelineLayout m_pipelineLayout;

	VkDescriptorSetLayout m_descriptorSetLayout;
	void createDescriptorPool();
	VkDescriptorPool m_descriptorPool;

	void createDescriptorSets(const std::vector<VkBuffer>& uniformBuffers);
	std::vector<VkDescriptorSet> m_descriptorSets;
};

#endif