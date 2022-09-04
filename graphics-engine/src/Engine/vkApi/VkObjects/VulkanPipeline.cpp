#include "VulkanPipeline.h"

#ifdef GRAPHICS_API_VULKAN

#include <Engine/vkApi/VKMacros.h>

#include <fstream>
#include <stdexcept>
#include <iostream>
#include <assert.h>

#define VK_NO_PROTOTYPES
#include <volk.h>

VulkanPipeline::VulkanPipeline(
	VulkanDevice& device,
	const std::string& vertFilepath,
	const std::string& fragFilepath,
	const VulkanPipelineConfigInfo* configInfo)
	: m_vulkanDevice(device)
{
	createGraphicsPipeline(vertFilepath, fragFilepath, configInfo);
}

VulkanPipeline::~VulkanPipeline()
{
	vkDestroyShaderModule(m_vulkanDevice.device(), m_vertShaderModule, nullptr);
	vkDestroyShaderModule(m_vulkanDevice.device(), m_fragShaderModule, nullptr);
	vkDestroyPipeline(m_vulkanDevice.device(), m_graphicsPipeline, nullptr);
}

void VulkanPipeline::bind(VkCommandBuffer commandBuffer)
{
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline);
}

VulkanPipelineConfigInfo* VulkanPipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height)
{
	VulkanPipelineConfigInfo* configInfo = new VulkanPipelineConfigInfo();

	configInfo->inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	configInfo->inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	configInfo->inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

	configInfo->viewport.x = 0.0f;
	configInfo->viewport.y = 0.0f;
	configInfo->viewport.width = static_cast<float>(width);
	configInfo->viewport.height = static_cast<float>(height);
	configInfo->viewport.minDepth = 0.0f;
	configInfo->viewport.maxDepth = 1.0f;

	configInfo->scissor.extent = { width, height };
	configInfo->scissor.offset = { 0, 0 };

	configInfo->rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	configInfo->rasterizationInfo.depthClampEnable = VK_FALSE;
	configInfo->rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
	configInfo->rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
	configInfo->rasterizationInfo.lineWidth = 1.0f;
	configInfo->rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
	configInfo->rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
	configInfo->rasterizationInfo.depthBiasEnable = VK_FALSE;
	configInfo->rasterizationInfo.depthBiasConstantFactor = 0.0f;
	configInfo->rasterizationInfo.depthBiasClamp = 0.0f;
	configInfo->rasterizationInfo.depthBiasSlopeFactor = 0.0f;

	configInfo->multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	configInfo->multisampleInfo.sampleShadingEnable = VK_FALSE;
	configInfo->multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	configInfo->multisampleInfo.minSampleShading = 1.0f;
	configInfo->multisampleInfo.pSampleMask = nullptr;
	configInfo->multisampleInfo.alphaToCoverageEnable = VK_FALSE;
	configInfo->multisampleInfo.alphaToOneEnable = VK_FALSE;

	configInfo->colorBlendAttachment = {};
	configInfo->colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
		VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_A_BIT;
	configInfo->colorBlendAttachment.blendEnable = VK_FALSE;
	configInfo->colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
	configInfo->colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
	configInfo->colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
	configInfo->colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
	configInfo->colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
	configInfo->colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional

	configInfo->colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	configInfo->colorBlendInfo.logicOpEnable = VK_FALSE;
	configInfo->colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
	configInfo->colorBlendInfo.attachmentCount = 1;
	configInfo->colorBlendInfo.pAttachments = &configInfo->colorBlendAttachment;
	configInfo->colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
	configInfo->colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
	configInfo->colorBlendInfo.blendConstants[2] = 0.0f;  // Optional
	configInfo->colorBlendInfo.blendConstants[3] = 0.0f;  // Optional

	configInfo->depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	configInfo->depthStencilInfo.depthTestEnable = VK_TRUE;
	configInfo->depthStencilInfo.depthWriteEnable = VK_TRUE;
	configInfo->depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
	configInfo->depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
	configInfo->depthStencilInfo.minDepthBounds = 0.0f;  // Optional
	configInfo->depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
	configInfo->depthStencilInfo.stencilTestEnable = VK_FALSE;
	configInfo->depthStencilInfo.front = {};  // Optional
	configInfo->depthStencilInfo.back = {};   // Optional

	return configInfo;
}

std::vector<char> VulkanPipeline::readFile(const std::string& filepath)
{
	std::ifstream file{ filepath, std::ios::ate | std::ios::binary };

	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open file " + filepath);
	}

	size_t size = static_cast<size_t>(file.tellg());
	std::vector<char> buffer(size);

	file.seekg(0);
	file.read(buffer.data(), size);

	file.close();

	return buffer;
}

void VulkanPipeline::createGraphicsPipeline(
	const std::string& vertFilepath,
	const std::string& fragFilepath,
	const VulkanPipelineConfigInfo* configInfo)
{
	assert(configInfo->pipelineLayout != VK_NULL_HANDLE && "Cannot create graphics pipeline without a pipeline layout");
	assert(configInfo->renderPass != VK_NULL_HANDLE && "Cannot create graphics pipeline without a render pass");

	auto vertCode = readFile(vertFilepath);
	auto fragCode = readFile(fragFilepath);

	createShaderModule(vertCode, &m_vertShaderModule);
	createShaderModule(fragCode, &m_fragShaderModule);

	VkPipelineShaderStageCreateInfo stages[2];
	stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	stages[0].module = m_vertShaderModule;
	stages[0].pName = "main";
	stages[0].flags = 0;
	stages[0].pNext = nullptr;
	stages[0].pSpecializationInfo = nullptr;


	stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	stages[1].module = m_fragShaderModule;
	stages[1].pName = "main";
	stages[1].flags = 0;
	stages[1].pNext = nullptr;
	stages[1].pSpecializationInfo = nullptr;

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexAttributeDescriptionCount = 0;
	vertexInputInfo.vertexBindingDescriptionCount = 0;
	vertexInputInfo.pVertexAttributeDescriptions = nullptr;
	vertexInputInfo.pVertexBindingDescriptions = nullptr;

	VkPipelineViewportStateCreateInfo viewportInfo{};
	viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportInfo.pScissors = &configInfo->scissor;
	viewportInfo.scissorCount = 1;
	viewportInfo.pViewports = &configInfo->viewport;
	viewportInfo.viewportCount = 1;

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = stages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &configInfo->inputAssemblyInfo;
	pipelineInfo.pViewportState = &viewportInfo;
	pipelineInfo.pRasterizationState = &configInfo->rasterizationInfo;
	pipelineInfo.pMultisampleState = &configInfo->multisampleInfo;
	pipelineInfo.pColorBlendState = &configInfo->colorBlendInfo;
	pipelineInfo.pDepthStencilState = &configInfo->depthStencilInfo;
	pipelineInfo.pDynamicState = nullptr;

	pipelineInfo.layout = configInfo->pipelineLayout;
	pipelineInfo.renderPass = configInfo->renderPass;
	pipelineInfo.subpass = configInfo->subpass;

	pipelineInfo.basePipelineIndex = -1;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	VK_CALL(vkCreateGraphicsPipelines(m_vulkanDevice.device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline));
}

void VulkanPipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule)
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
	createInfo.codeSize = code.size();

	VK_CALL(vkCreateShaderModule(m_vulkanDevice.device(), &createInfo, nullptr, shaderModule));
}

#endif