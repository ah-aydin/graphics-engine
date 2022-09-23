#include "BasicRenderSystem.h"

#ifdef GRAPHICS_API_VULKAN

#include <Engine/vkApi/VKMacros.h>
#include <Engine/vkApi/VkObjects/VulkanSwapchain.h>
#include <Engine/vkApi/Rendering/UniformBufferObjects/UniformBufferObject3D.h>

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
	alignas(16) glm::mat4 model{};
	alignas(16) glm::mat4 view{};
	alignas(16) glm::mat4 proj{};
};

BasicRenderSystem::BasicRenderSystem(
	VulkanDevice &device,
	VkRenderPass renderPass,
	std::string vertFilePath,
	std::string fragFilePath,
	const std::vector<VkBuffer>& uniformBuffers,
	RenderDimention renderDimention
)
	: m_vulkanDevice(device), m_renderDimention(renderDimention)
{
	createPipelineLayout();
	createPipeline(renderPass, renderDimention, vertFilePath, fragFilePath);
	createDescriptorPool();
	createDescriptorSets(uniformBuffers);
}

BasicRenderSystem::~BasicRenderSystem()
{
	vkDestroyDescriptorPool(m_vulkanDevice.device(), m_descriptorPool, nullptr);
	vkDestroyDescriptorSetLayout(m_vulkanDevice.device(), m_descriptorSetLayout, nullptr);
	vkDestroyPipelineLayout(m_vulkanDevice.device(), m_pipelineLayout, nullptr);
}

void BasicRenderSystem::createPipelineLayout()
{
	// Push constants
	VkPushConstantRange pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	if (m_renderDimention == RENDER2D)		pushConstantRange.size = sizeof(SimplePushConstantData2D);
	else if (m_renderDimention == RENDER3D) pushConstantRange.size = sizeof(SimplePushConstantData3D);

	// Uniform buffers

	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo{};
	descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSetLayoutCreateInfo.bindingCount = 1;
	descriptorSetLayoutCreateInfo.pBindings = &uboLayoutBinding;

	VK_CALL(vkCreateDescriptorSetLayout(m_vulkanDevice.device(), &descriptorSetLayoutCreateInfo, nullptr, &m_descriptorSetLayout));

	// Pipeline layout
	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.setLayoutCount = 0;
	pipelineLayoutCreateInfo.pSetLayouts = nullptr;
	pipelineLayoutCreateInfo.pushConstantRangeCount = 1;
	pipelineLayoutCreateInfo.pPushConstantRanges = &pushConstantRange;
	pipelineLayoutCreateInfo.setLayoutCount = 1;
	pipelineLayoutCreateInfo.pSetLayouts = &m_descriptorSetLayout;

	VK_CALL(vkCreatePipelineLayout(m_vulkanDevice.device(), &pipelineLayoutCreateInfo, nullptr, &m_pipelineLayout));
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

void BasicRenderSystem::createDescriptorPool()
{
	VkDescriptorPoolSize poolSize{};
	poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize.descriptorCount = static_cast<uint32_t>(VulkanSwapchain::MAX_FRAMES_IN_FLIGHT);

	VkDescriptorPoolCreateInfo poolCreateInfo{};
	poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolCreateInfo.poolSizeCount = 1;
	poolCreateInfo.pPoolSizes = &poolSize;
	poolCreateInfo.maxSets = static_cast<uint32_t>(VulkanSwapchain::MAX_FRAMES_IN_FLIGHT);

	VK_CALL(vkCreateDescriptorPool(m_vulkanDevice.device(), &poolCreateInfo, nullptr, &m_descriptorPool));
}

void BasicRenderSystem::createDescriptorSets(const std::vector<VkBuffer>& uniformBuffers)
{
	std::vector<VkDescriptorSetLayout> layouts(VulkanSwapchain::MAX_FRAMES_IN_FLIGHT, m_descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = m_descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(VulkanSwapchain::MAX_FRAMES_IN_FLIGHT);
	allocInfo.pSetLayouts = layouts.data();

	m_descriptorSets.resize(VulkanSwapchain::MAX_FRAMES_IN_FLIGHT);
	VK_CALL(vkAllocateDescriptorSets(m_vulkanDevice.device(), &allocInfo, m_descriptorSets.data()));

	for (size_t i = 0; i < VulkanSwapchain::MAX_FRAMES_IN_FLIGHT; ++i)
	{
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject3D);
		
		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = m_descriptorSets[i];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;
		descriptorWrite.pImageInfo = nullptr; // Optional
		descriptorWrite.pTexelBufferView = nullptr; // Optional

		vkUpdateDescriptorSets(m_vulkanDevice.device(), 1, &descriptorWrite, 0, nullptr);
	}
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

void BasicRenderSystem::renderGameObjects3D(
	VkCommandBuffer commandBuffer,
	VkDeviceMemory uniformBufferMemory,
	int currentImageIndex,
	std::vector<VKGameObject3D>& gameObjects
)
{
	m_vulkanPipeline->bind(commandBuffer);

	for (auto& obj : gameObjects)
	{
		obj.m_transform.rotation.y = glm::mod(obj.m_transform.rotation.y + 1000.f * (float) Time::getDeltaTime(), glm::two_pi<float>());
		obj.m_transform.rotation.x = glm::mod(obj.m_transform.rotation.x + 700.f * (float) Time::getDeltaTime(), glm::two_pi<float>());

		SimplePushConstantData3D push{};
		push.model = obj.m_transform.mat4();
		push.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		push.proj = glm::perspective(glm::radians(45.f), 16.f / 9.f, 0.1f, 100.f);
		push.proj[1][1] *= -1;

		vkCmdPushConstants(
			commandBuffer,
			m_pipelineLayout,
			VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			0,
			sizeof(SimplePushConstantData3D),
			&push
		);

		UniformBufferObject3D ubo{};
		ubo.model = obj.m_transform.mat4();
		ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.proj = glm::perspective(glm::radians(45.f), 16.f / 9.f, 0.1f, 100.f);
		ubo.proj[1][1] *= -1;

		void* data;
		vkMapMemory(m_vulkanDevice.device(), uniformBufferMemory, 0, sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(m_vulkanDevice.device(), uniformBufferMemory);
		
		obj.m_model->bind(commandBuffer);
		vkCmdBindDescriptorSets(
			commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			m_pipelineLayout,
			0,
			1,
			&m_descriptorSets[currentImageIndex],
			0,
			nullptr
		);
		obj.m_model->draw(commandBuffer);
	}
}

#endif