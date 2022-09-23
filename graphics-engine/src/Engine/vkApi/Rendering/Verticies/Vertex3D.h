#pragma once

#ifdef GRAPHICS_API_VULKAN

#define VK_NO_PROTOTYPES
#include <volk.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

struct Vertex3D
{
	glm::vec3 position;

	static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
	static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
};

#endif