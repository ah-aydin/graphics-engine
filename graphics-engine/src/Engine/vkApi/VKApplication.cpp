#include "VKApplication.h"

#ifdef GRAPHICS_API_VULKAN

#include "VKMacros.h"

#include <Engine/Input.h>
#include <Engine/Time.h>

#include <stdexcept>

#define VK_NO_PROTOTYPES
#include <volk.h>

VKApplication::VKApplication() : Application()
{
	Input::createAction("QUIT", GLFW_KEY_ESCAPE, false);
	createPipelineLayout();
	createPipeline();
	createCommandBuffers();
}

VKApplication::~VKApplication()
{
	vkDestroyPipelineLayout(m_vulkanDevice.device(), m_pipelineLayout, nullptr);
}

void VKApplication::run()
{
	while (!m_window.shouldClose())
	{
		if (Input::getAction("QUIT"))
		{
			m_window.setShouldClose(true);
		}

		drawFrame();
		glfwPollEvents();
		Time::tick();
	}

	vkDeviceWaitIdle(m_vulkanDevice.device());
}

void VKApplication::drawFrame()
{
	uint32_t imageIndex;
	auto result = m_vulkanSwapchain.acquireNextImage(&imageIndex);

	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("Failed to aquire next swapchain image");
	}

	result = m_vulkanSwapchain.submitCommandBuffers(&m_commandBuffers[imageIndex], &imageIndex);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to present swapchain");
	}
}

void VKApplication::createPipelineLayout()
{
	VkPipelineLayoutCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	createInfo.setLayoutCount = 0;
	createInfo.pSetLayouts = nullptr;
	createInfo.pushConstantRangeCount = 0;
	createInfo.pPushConstantRanges = nullptr;

	VK_CALL(vkCreatePipelineLayout(m_vulkanDevice.device(), &createInfo, nullptr, &m_pipelineLayout));
}

void VKApplication::createPipeline()
{
	auto pipelineConfig = VulkanPipeline::defaultPipelineConfigInfo(m_vulkanSwapchain.width(), m_vulkanSwapchain.height());
	pipelineConfig->renderPass = m_vulkanSwapchain.getRenderPass();
	pipelineConfig->pipelineLayout = m_pipelineLayout;
	m_vulkanPipeline = std::make_unique<VulkanPipeline>(
		m_vulkanDevice,
		"res/vulkan/basic/shader.vert.spv",
		"res/vulkan/basic/shader.frag.spv",
		pipelineConfig
		);
	delete pipelineConfig;
}

void VKApplication::createCommandBuffers()
{
	m_commandBuffers.resize(m_vulkanSwapchain.imageCount());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = m_vulkanDevice.getCommandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());

	VK_CALL(vkAllocateCommandBuffers(m_vulkanDevice.device(), &allocInfo, m_commandBuffers.data()));

	for (int i = 0; i < m_commandBuffers.size(); ++i)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		VK_CALL(vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo));

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_vulkanSwapchain.getRenderPass();
		renderPassInfo.framebuffer = m_vulkanSwapchain.getFrameBuffer(i);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_vulkanSwapchain.getSwapChainExtent();

		std::vector<VkClearValue> clearValues;
		clearValues.resize(2);
		clearValues[0].color = { 0, 0, 0, 1 };
		clearValues[1].depthStencil = { 1.0f, 0 };

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(m_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		m_vulkanPipeline->bind(m_commandBuffers[i]);
		vkCmdDraw(m_commandBuffers[i], 3, 1, 0, 0);

		vkCmdEndRenderPass(m_commandBuffers[i]);

		VK_CALL(vkEndCommandBuffer(m_commandBuffers[i]));
	}
}

#endif