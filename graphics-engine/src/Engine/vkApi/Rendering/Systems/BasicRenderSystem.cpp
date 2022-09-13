#include "BasicRenderSystem.h"

#ifdef GRAPHICS_API_VULKAN

#include <Engine/vkApi/VKMacros.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <stdexcept>

#define VK_NO_PROTOTYPES
#include <volk.h>

struct SimplePushConstantData2D
{
	glm::mat2 transform{ 1.0f };
	glm::vec2 offset;
	alignas(16) glm::vec3 color;
};

struct SimplePushConstantData3D
{
	glm::mat4 transform{ 1.0f };
	alignas(16) glm::vec3 color;
};

BasicRenderSystem::BasicRenderSystem(
	VulkanDevice &device,
	VkRenderPass renderPass,
	std::string vertFilePath,
	std::string fragFilePath,
	RenderDimention renderDimention
)
	: m_vulkanDevice(device), m_renderDimention(renderDimention)
{
	createPipelineLayout();
	createPipeline(renderPass, renderDimention, vertFilePath, fragFilePath);
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
	if (m_renderDimention == RENDER2D)		pushConstantRange.size = sizeof(SimplePushConstantData2D);
	else if (m_renderDimention == RENDER3D) pushConstantRange.size = sizeof(SimplePushConstantData3D);

	VkPipelineLayoutCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	createInfo.setLayoutCount = 0;
	createInfo.pSetLayouts = nullptr;
	createInfo.pushConstantRangeCount = 1;
	createInfo.pPushConstantRanges = &pushConstantRange;

	VK_CALL(vkCreatePipelineLayout(m_vulkanDevice.device(), &createInfo, nullptr, &m_pipelineLayout));
}

void BasicRenderSystem::createPipeline(
	VkRenderPass renderPass,
	RenderDimention renderDimention,
	std::string vertFilePath,
	std::string fragFilePath
)
{
	assert(m_pipelineLayout != nullptr && "A pipeline layout should be given to create pipeline");

	VulkanPipelineConfigInfo pipelineConfig{};
	VulkanPipeline::defaultPipelineConfigInfo(pipelineConfig);

	pipelineConfig.renderPass = renderPass;
	pipelineConfig.pipelineLayout = m_pipelineLayout;
	m_vulkanPipeline = std::make_unique<VulkanPipeline>(
		m_vulkanDevice,
		vertFilePath,
		fragFilePath,
		pipelineConfig,
		renderDimention
	);
}

#include <Engine/Time.h>
void BasicRenderSystem::renderGameObjects2D(VkCommandBuffer commandBuffer, std::vector<VKGameObject2D> &gameObjects)
{
	m_vulkanPipeline->bind(commandBuffer);

	for (auto& obj : gameObjects)
	{
		obj.m_transform.rotation += 1000.f * (float) Time::getDeltaTime();

		SimplePushConstantData2D push{};
		push.offset = obj.m_transform.translation;
		push.color = obj.m_color;
		push.transform = obj.m_transform.mat2();

		vkCmdPushConstants(
			commandBuffer,
			m_pipelineLayout,
			VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			0,
			sizeof(SimplePushConstantData2D),
			&push
		);
		obj.m_model->bind(commandBuffer);
		obj.m_model->draw(commandBuffer);
	}
}

void BasicRenderSystem::renderGameObjects3D(VkCommandBuffer commandBuffer, std::vector<VKGameObject3D>& gameObjects)
{
	m_vulkanPipeline->bind(commandBuffer);

	for (auto& obj : gameObjects)
	{
		obj.m_transform.rotation.y = glm::mod(obj.m_transform.rotation.y + 1000.f * (float) Time::getDeltaTime(), glm::two_pi<float>());
		obj.m_transform.rotation.x = glm::mod(obj.m_transform.rotation.x + 700.f * (float) Time::getDeltaTime(), glm::two_pi<float>());

		SimplePushConstantData3D push{};
		push.color = obj.m_color;
		push.transform = obj.m_transform.mat4();

		vkCmdPushConstants(
			commandBuffer,
			m_pipelineLayout,
			VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			0,
			sizeof(SimplePushConstantData3D),
			&push
		);
		obj.m_model->bind(commandBuffer);
		obj.m_model->draw(commandBuffer);
	}
}

#endif