#pragma once

#ifdef GRAPHICS_API_VULKAN


#include <string>
#include <vector>

#include "VulkanDevice.h"

struct VulkanPipelineConfigInfo {
	VkViewport viewport;
	VkRect2D scissor;
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
	VkPipelineRasterizationStateCreateInfo rasterizationInfo;
	VkPipelineMultisampleStateCreateInfo multisampleInfo;
	VkPipelineColorBlendAttachmentState colorBlendAttachment;
	VkPipelineColorBlendStateCreateInfo colorBlendInfo;
	VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
	VkPipelineLayout pipelineLayout = nullptr;
	VkRenderPass renderPass = nullptr;
	uint32_t subpass = 0;
};

class VulkanPipeline
{
public:
	VulkanPipeline(
		VulkanDevice& device,
		const std::string& vertFilepath,
		const std::string& fragFilepath,
		const VulkanPipelineConfigInfo* configInfo);
	~VulkanPipeline();

	VulkanPipeline(const VulkanPipeline&) = delete;
	VulkanPipeline operator=(const VulkanPipeline&) = delete;

	void bind(VkCommandBuffer commandBuffer);
	static VulkanPipelineConfigInfo* defaultPipelineConfigInfo(uint32_t width, uint32_t height);

private:
	static std::vector<char> readFile(const std::string& filepath);
	void createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, const VulkanPipelineConfigInfo* configInfo);

	void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

	VulkanDevice& m_vulkanDevice;
	VkPipeline m_graphicsPipeline;
	VkShaderModule m_vertShaderModule;
	VkShaderModule m_fragShaderModule;
};

#endif