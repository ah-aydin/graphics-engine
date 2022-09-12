#include "VulkanRenderer.h"

#ifdef GRAPHICS_API_VULKAN

#include <Engine/vkApi/VKMacros.h>

#include <stdexcept>

#define VK_NO_PROTOTYPES
#include <volk.h>


VulkanRenderer::VulkanRenderer(VKWindow &window, VulkanDevice &device)
	: m_window(window), m_vulkanDevice(device)
{
	recreateSwapchain();
	createCommandBuffers();
}

VulkanRenderer::~VulkanRenderer()
{
	freeCommandBuffers();
}

void VulkanRenderer::createCommandBuffers()
{
	m_commandBuffers.resize(VulkanSwapchain::MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = m_vulkanDevice.getCommandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());

	VK_CALL(vkAllocateCommandBuffers(m_vulkanDevice.device(), &allocInfo, m_commandBuffers.data()));
}

void VulkanRenderer::freeCommandBuffers()
{
	vkFreeCommandBuffers(m_vulkanDevice.device(), m_vulkanDevice.getCommandPool(), static_cast<uint32_t>(m_commandBuffers.size()), m_commandBuffers.data());
	m_commandBuffers.clear();
}

void VulkanRenderer::recreateSwapchain()
{
	log_info("Creating swapchain");
	vkDeviceWaitIdle(m_vulkanDevice.device());
	m_vulkanSwapchain.reset();
	auto extent = m_window.getExtent();
	while (extent.width == 0 || extent.height == 0)
	{
		extent = m_window.getExtent();
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(m_vulkanDevice.device());

	if (m_vulkanSwapchain == nullptr)
	{
		m_vulkanSwapchain = std::make_unique<VulkanSwapchain>(m_vulkanDevice, extent);
	}
	else
	{
		std::shared_ptr<VulkanSwapchain> oldSwapchain = std::move(m_vulkanSwapchain);
		m_vulkanSwapchain = std::make_unique<VulkanSwapchain>(m_vulkanDevice, extent, oldSwapchain);

		if (!oldSwapchain->compareSwapFormats(*m_vulkanSwapchain.get()))
		{
			throw std::runtime_error("Swapchain format has changed");
		}

		//if (m_vulkanSwapchain->imageCount() != m_commandBuffers.size())
		//{
		//	freeCommandBuffers();
		//	createCommandBuffers();
		//}
	}

	log_info("Done!");
}

VkCommandBuffer VulkanRenderer::beginFrame()
{
	assert(!m_isFrameStarted && "Cannot begin frame while another frame is in progress");
	
	auto result = m_vulkanSwapchain->acquireNextImage(&m_currentImageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		recreateSwapchain();
		return nullptr;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("Failed to aquire next swapchain image");
	}

	m_isFrameStarted = true;

	auto commandBuffer = getCurrentCommandBuffer();
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	VK_CALL(vkBeginCommandBuffer(commandBuffer, &beginInfo));

	return commandBuffer;
}

void VulkanRenderer::endFrame(VkCommandBuffer commandBuffer)
{
	assert(m_isFrameStarted && "Cannot end frame while there is no frame is in progress");
	assert(commandBuffer == getCurrentCommandBuffer() && "Cannot end frame command buffer from another frame");
	
	VK_CALL(vkEndCommandBuffer(commandBuffer));

	auto result = m_vulkanSwapchain->submitCommandBuffers(&commandBuffer, &m_currentImageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_window.wasResized())
	{
		m_window.resetResizedFlag();
		recreateSwapchain();
	}
	else if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to present swapchain");
	}

	m_isFrameStarted = false;
	m_currentFrameIndex = (m_currentFrameIndex + 1) % VulkanSwapchain::MAX_FRAMES_IN_FLIGHT;
}

void VulkanRenderer::beginSwapchainRenderPass(VkCommandBuffer commandBuffer)
{
	assert(m_isFrameStarted && "Cannot begin swapchain render pass while there is no frame is in progress");
	assert(commandBuffer == getCurrentCommandBuffer() && "Cannot begin render pass on command buffer from another frame");

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = m_vulkanSwapchain->getRenderPass();
	renderPassInfo.framebuffer = m_vulkanSwapchain->getFrameBuffer(m_currentImageIndex);

	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = m_vulkanSwapchain->getSwapChainExtent();

	std::vector<VkClearValue> clearValues;
	clearValues.resize(2);
	clearValues[0].color = { 0, 0, 0, 1 };
	clearValues[1].depthStencil = { 1.0f, 0 };

	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport viewport{};
	viewport.x = 0;
	viewport.y = 0;
	viewport.width = static_cast<float>(m_vulkanSwapchain->getSwapChainExtent().width);
	viewport.height = static_cast<float>(m_vulkanSwapchain->getSwapChainExtent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{ {0, 0}, m_vulkanSwapchain->getSwapChainExtent() };
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void VulkanRenderer::endSwapchainRenderPass(VkCommandBuffer commandBuffer)
{
	assert(m_isFrameStarted && "Cannot end swapchain render pass while there is no frame is in progress");
	assert(commandBuffer == getCurrentCommandBuffer() && "Cannot end render pass on command buffer from another frame");
	
	vkCmdEndRenderPass(commandBuffer);
}

#endif