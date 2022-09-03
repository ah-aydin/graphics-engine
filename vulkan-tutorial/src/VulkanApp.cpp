#include "VulkanApp.h"

#include <stdexcept>
#include <array>

VulkanApp::VulkanApp()
{
	createPipelineLayout();
	createPipeline();
	createCommandBuffers();
}

VulkanApp::~VulkanApp()
{
	vkDestroyPipelineLayout(vulkanDevice.device(), pipelineLayout, nullptr);
}

void VulkanApp::run()
{
	while (!vulkanWindow.shouldClose())
	{
		glfwPollEvents();
		drawFrame();
	}

	vkDeviceWaitIdle(vulkanDevice.device());
}

void VulkanApp::createPipelineLayout()
{
	VkPipelineLayoutCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	createInfo.setLayoutCount = 0;
	createInfo.pSetLayouts = nullptr;
	createInfo.pushConstantRangeCount = 0;
	createInfo.pPushConstantRanges = nullptr;

	if (vkCreatePipelineLayout(vulkanDevice.device(), &createInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create pipeline layout");
	}
}

void VulkanApp::createPipeline()
{
	auto pipelineConfig = VulkanPipeline::defaultPipelineConfigInfo(vulkanSwapchain.width(), vulkanSwapchain.height());
	pipelineConfig.renderPass = vulkanSwapchain.getRenderPass();
	pipelineConfig.pipelineLayout = pipelineLayout;
	vulkanPipeline = std::make_unique<VulkanPipeline>(
		vulkanDevice,
		"res/vert.spv",
		"res/frag.spv",
		pipelineConfig
	);
}

void VulkanApp::createCommandBuffers()
{
	commandBuffers.resize(vulkanSwapchain.imageCount());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = vulkanDevice.getCommandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

	if (vkAllocateCommandBuffers(vulkanDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to allocate command buffers");
	}

	for (int i = 0; i < commandBuffers.size(); ++i)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to begin recording command buffer");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = vulkanSwapchain.getRenderPass();
		renderPassInfo.framebuffer = vulkanSwapchain.getFrameBuffer(i);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = vulkanSwapchain.getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0, 0, 0, 1 };
		clearValues[1].depthStencil = { 1.0f, 0 };

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		vulkanPipeline->bind(commandBuffers[i]);
		vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

		vkCmdEndRenderPass(commandBuffers[i]);

		if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to record command buffer");
		}
	}
}

void VulkanApp::drawFrame()
{
	uint32_t imageIndex;
	auto result = vulkanSwapchain.acquireNextImage(&imageIndex);

	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("Failed to aquire next swapchain image");
	}

	result = vulkanSwapchain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to present swapchain");
	}
}
