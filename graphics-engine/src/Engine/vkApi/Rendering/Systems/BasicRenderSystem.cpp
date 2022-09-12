#include "BasicRenderSystem.h"

#ifdef GRAPHICS_API_VULKAN

#include <Engine/vkApi/VKMacros.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <stdexcept>

#define VK_NO_PROTOTYPES
#include <volk.h>

struct SimplePushConstantData
{
	glm::mat2 transform{ 1.0f };
	glm::vec2 offset;
	alignas(16) glm::vec3 color;
};

BasicRenderSystem::BasicRenderSystem(VulkanDevice &device, VkRenderPass renderPass)
	: m_vulkanDevice(device)
{
	createPipelineLayout();
	createPipeline(renderPass);
}

BasicRenderSystem::~BasicRenderSystem()
{
	vkDestroyPipelineLayout(m_vulkanDevice.device(), m_pipelineLayout, nullptr);
}

void BasicRenderSystem::createPipelineLayout()
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

void BasicRenderSystem::createPipeline(VkRenderPass renderPass)
{
	assert(m_pipelineLayout != nullptr && "A pipeline layout should be given to create pipeline");

	VulkanPipelineConfigInfo pipelineConfig{};
	VulkanPipeline::defaultPipelineConfigInfo(pipelineConfig);

	pipelineConfig.renderPass = renderPass;
	pipelineConfig.pipelineLayout = m_pipelineLayout;
	m_vulkanPipeline = std::make_unique<VulkanPipeline>(
		m_vulkanDevice,
		"res/vulkan/basic/shader.vert.spv",
		"res/vulkan/basic/shader.frag.spv",
		pipelineConfig
		);
}

void BasicRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<VKGameObject2D> &gameObjects)
{
	m_vulkanPipeline->bind(commandBuffer);

	for (auto& obj : gameObjects)
	{
		SimplePushConstantData push{};
		push.offset = obj.m_transform.translation;
		push.color = obj.m_color;
		push.transform = obj.m_transform.mat2();

		vkCmdPushConstants(
			commandBuffer,
			m_pipelineLayout,
			VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			0,
			sizeof(SimplePushConstantData),
			&push
		);
		obj.m_model->bind(commandBuffer);
		obj.m_model->draw(commandBuffer);
	}
}

#endif