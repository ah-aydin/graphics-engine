#pragma once

#ifdef GRAPHICS_API_VULKAN


#include <string>
#include <vector>

#include "VulkanDevice.h"

struct VulkanPipelineConfigInfo {
	VulkanPipelineConfigInfo();
	VulkanPipelineConfigInfo(const VulkanPipelineConfigInfo&) = delete;
	VulkanPipelineConfigInfo& operator=(const VulkanPipelineConfigInfo&) = delete;

	VkPipelineViewportStateCreateInfo viewportInfo;
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
	VkPipelineRasterizationStateCreateInfo rasterizationInfo;
	VkPipelineMultisampleStateCreateInfo multisampleInfo;
	VkPipelineColorBlendAttachmentState colorBlendAttachment;
	VkPipelineColorBlendStateCreateInfo colorBlendInfo;
	VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
	std::vector<VkDynamicState> dynamicStateEnables;
	VkPipelineDynamicStateCreateInfo dynamicStateInfo;
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
		const VulkanPipelineConfigInfo& configInfo);
	~VulkanPipeline();

	VulkanPipeline(const VulkanPipeline&) = delete;
	VulkanPipeline operator=(const VulkanPipeline&) = delete;

	void bind(VkCommandBuffer commandBuffer);
	static void defaultPipelineConfigInfo(VulkanPipelineConfigInfo& configInfo);

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