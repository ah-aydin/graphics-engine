#include "Vertex2D.h"

#ifdef GRAPHICS_API_VULKAN

#include <cassert>

std::vector<VkVertexInputBindingDescription> Vertex2D::getBindingDescriptions()
{
	std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);

	bindingDescriptions[0].binding = 0;
	bindingDescriptions[0].stride = sizeof(Vertex2D);
	bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription> Vertex2D::getAttributeDescriptions()
{
	return {
		// { location, binding, format, offset }
		{ 0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex2D, position) },
	};
}

#endif