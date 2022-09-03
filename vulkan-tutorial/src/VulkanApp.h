#pragma once

#include "VulkanWindow.h"
#include "VulkanPipeline.h"
#include "VulkanDevice.h"
#include "VulkanSwapchain.h"

#include <memory>
#include <vector>

class VulkanApp
{
public:
	static constexpr int WIDTH = 800;
	static constexpr int HEIGHT = 600;

	VulkanApp();
	~VulkanApp();

	VulkanApp(const VulkanApp&) = delete;
	VulkanApp operator=(const VulkanApp&) = delete;

	void run();

private:
	void createPipelineLayout();
	void createPipeline();
	void createCommandBuffers();

	void drawFrame();
	
	VulkanWindow vulkanWindow{ WIDTH, HEIGHT, "VULKAN TUTORIAL" };
	VulkanDevice vulkanDevice{ vulkanWindow };
	VulkanSwapchain vulkanSwapchain{ vulkanDevice, vulkanWindow.getExtent() };

	std::unique_ptr<VulkanPipeline> vulkanPipeline;
	VkPipelineLayout pipelineLayout;
	std::vector<VkCommandBuffer> commandBuffers;
};

