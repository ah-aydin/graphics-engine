#pragma once

#include <vector>

#include <vulkan/vulkan.h>

struct ShaderModule
{
	std::vector<unsigned int> SPIRV;
	VkShaderModule shaderModule;
};