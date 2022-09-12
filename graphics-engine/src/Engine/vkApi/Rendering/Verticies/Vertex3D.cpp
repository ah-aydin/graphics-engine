#include "Vertex3D.h"

#ifdef GRAPHICS_API_VULKAN

#include <cassert>

std::vector<VkVertexInputBindingDescription> Vertex3D::getBindingDescriptions()
{
	std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);

	bindingDescriptions[0].binding = 0;
	bindingDescriptions[0].stride = sizeof(Vertex3D);
	bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription> Vertex3D::getAttributeDescriptions()
{
	return {
		// { location, binding, format, offset }
		{ 0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex3D, position) },
		{ 1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex3D, color) }
	};
}

#endif