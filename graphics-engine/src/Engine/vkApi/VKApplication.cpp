#include "VKApplication.h"

#ifdef GRAPHICS_API_VULKAN

#include "VKMacros.h"

#include <Engine/Input.h>
#include <Engine/Time.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <stdexcept>

#define VK_NO_PROTOTYPES
#include <volk.h>

struct SimplePushConstantData
{
	glm::vec2 offset;
	alignas(16) glm::vec3 color;
};

VKApplication::VKApplication() : Application()
{
	loadModels();

	Input::createAction("QUIT", GLFW_KEY_ESCAPE, false);
	createPipelineLayout();
	reacreateSwapchain();
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
	auto result = m_vulkanSwapchain->acquireNextImage(&imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		reacreateSwapchain();
		return;
	}

	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("Failed to aquire next swapchain image");
	}

	recordCommandBuffer(imageIndex);
	result = m_vulkanSwapchain->submitCommandBuffers(&m_commandBuffers[imageIndex], &imageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_window.wasResized())
	{
		m_window.resetResizedFlag();
		reacreateSwapchain();
		return;
	}
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to present swapchain");
	}
}

void VKApplication::createPipelineLayout()
{
	VkPushConstantRange pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(SimplePushConstantData);

	VkPipelineLayoutCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	createInfo.setLayoutCount = 0;
	createInfo.pSetLayouts = nullptr;
	createInfo.pushConstantRangeCount = 1;
	createInfo.pPushConstantRanges = &pushConstantRange;

	VK_CALL(vkCreatePipelineLayout(m_vulkanDevice.device(), &createInfo, nullptr, &m_pipelineLayout));
}

void VKApplication::createPipeline()
{
	assert(m_vulkanSwapchain != nullptr && "A vulkan swapchain should be given to create pipeline");
	assert(m_pipelineLayout != nullptr && "A pipeline layout should be given to create pipeline");

	VulkanPipelineConfigInfo pipelineConfig{};
	VulkanPipeline::defaultPipelineConfigInfo(pipelineConfig);
	
	pipelineConfig.renderPass = m_vulkanSwapchain->getRenderPass();
	pipelineConfig.pipelineLayout = m_pipelineLayout;
	m_vulkanPipeline = std::make_unique<VulkanPipeline>(
		m_vulkanDevice,
		"res/vulkan/basic/shader.vert.spv",
		"res/vulkan/basic/shader.frag.spv",
		pipelineConfig
		);
}

void VKApplication::createCommandBuffers()
{
	m_commandBuffers.resize(m_vulkanSwapchain->imageCount());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = m_vulkanDevice.getCommandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());

	VK_CALL(vkAllocateCommandBuffers(m_vulkanDevice.device(), &allocInfo, m_commandBuffers.data()));
}

void VKApplication::freeCommandBuffers()
{
	vkFreeCommandBuffers(m_vulkanDevice.device(), m_vulkanDevice.getCommandPool(), static_cast<uint32_t>(m_commandBuffers.size()), m_commandBuffers.data());
	m_commandBuffers.clear();
}

glm::vec3 leftColor = { 1, 0, 0 };
glm::vec3 rightColor = { 0, 1, 0 };
glm::vec3 centerColor = { 0, 0, 1 };
void sierpinski(std::vector<VKModel::Vertex> &verticies, int depth, glm::vec2 left, glm::vec2 right, glm::vec2 center)
{
	if (depth <= 0)
	{
		verticies.push_back({ left, leftColor });
		verticies.push_back({ right, rightColor });
		verticies.push_back({ center, centerColor });
		return;
	}

	auto newRight = 0.5f * (right + center);
	auto newLeft = 0.5f * (left + center);
	auto newCenter = 0.5f * (left + right);
	sierpinski(verticies, depth - 1, left, newCenter, newLeft);
	sierpinski(verticies, depth - 1, newCenter, right, newRight);
	sierpinski(verticies, depth - 1, newLeft, newRight, center);
}

void VKApplication::loadModels()
{
	std::vector<VKModel::Vertex> verticiesSierpinski{};
	sierpinski(verticiesSierpinski, 3, { -0.5f, 0.5f }, { 0.5f, 0.5f }, { 0.0f, -0.5f });
	std::vector<VKModel::Vertex> verticiesTriangle {
		{{ -0.5f, 0.5f }}, {{ 0.5f, 0.5f }}, {{ 0.0f, -0.5f }}
	};
	m_model = std::make_unique<VKModel>(m_vulkanDevice, verticiesTriangle);
}

void VKApplication::reacreateSwapchain()
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
		m_vulkanSwapchain = std::make_unique<VulkanSwapchain>(m_vulkanDevice, extent, std::move(m_vulkanSwapchain));
		if (m_vulkanSwapchain->imageCount() != m_commandBuffers.size())
		{
			freeCommandBuffers();
			createCommandBuffers();
		}
	}
	createPipeline();
	log_info("Done!");
}

double speed = 50.0f;
double xPos = 0.0f;
void VKApplication::recordCommandBuffer(int imageIndex)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	VK_CALL(vkBeginCommandBuffer(m_commandBuffers[imageIndex], &beginInfo));

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = m_vulkanSwapchain->getRenderPass();
	renderPassInfo.framebuffer = m_vulkanSwapchain->getFrameBuffer(imageIndex);

	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = m_vulkanSwapchain->getSwapChainExtent();

	std::vector<VkClearValue> clearValues;
	clearValues.resize(2);
	clearValues[0].color = { 0, 0, 0, 1 };
	clearValues[1].depthStencil = { 1.0f, 0 };

	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(m_commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport viewport{};
	viewport.x = 0;
	viewport.y = 0;
	viewport.width = static_cast<float>(m_vulkanSwapchain->getSwapChainExtent().width);
	viewport.height = static_cast<float>(m_vulkanSwapchain->getSwapChainExtent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{ {0, 0}, m_vulkanSwapchain->getSwapChainExtent() };
	vkCmdSetViewport(m_commandBuffers[imageIndex], 0, 1, &viewport);
	vkCmdSetScissor(m_commandBuffers[imageIndex], 0, 1, &scissor);

	m_vulkanPipeline->bind(m_commandBuffers[imageIndex]);
	m_model->bind(m_commandBuffers[imageIndex]);

	for (int j = 0; j < 4; j++)
	{
		SimplePushConstantData push{};
		xPos += speed * Time::getDeltaTime();
		log_info("%lf", xPos);
		if (xPos >= .5f) speed = -speed;
		if (xPos <= -.5f) speed = -speed;
		push.offset = { xPos, -0.4f + j * 0.25f};
		push.color = { 0.0f, 0.0f, 0.2f + 0.2f * j };

		vkCmdPushConstants(
			m_commandBuffers[imageIndex],
			m_pipelineLayout,
			VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			0,
			sizeof(SimplePushConstantData),
			&push
		);
		m_model->draw(m_commandBuffers[imageIndex]);
	}

	vkCmdEndRenderPass(m_commandBuffers[imageIndex]);

	VK_CALL(vkEndCommandBuffer(m_commandBuffers[imageIndex]));
}

#endif