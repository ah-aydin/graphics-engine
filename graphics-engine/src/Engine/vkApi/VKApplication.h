#pragma once

#ifdef GRAPHICS_API_VULKAN

#define VK_NO_PROTOTYPES
#include <Engine/Common/Application.h>

#include "VKWindow.h"
#include "VkObjects/VulkanPipeline.h"
#include "VkObjects/VulkanDevice.h"
#include "VkObjects/VulkanSwapchain.h"

#include <vector>
#include <memory>

class VKApplication : public Application
{
public:
	VKApplication();
	~VKApplication();

	void run() override;

private:
	void drawFrame();

	VKWindow m_window{ "Vulkan Application" };
	VulkanDevice m_vulkanDevice{ m_window };
	VulkanSwapchain m_vulkanSwapchain{ m_vulkanDevice, m_window.getExtent() };

	void createPipelineLayout();
	void createPipeline();
	void createCommandBuffers();

	std::unique_ptr<VulkanPipeline> m_vulkanPipeline;
	VkPipelineLayout m_pipelineLayout;
	std::vector<VkCommandBuffer> m_commandBuffers;
};

#endif