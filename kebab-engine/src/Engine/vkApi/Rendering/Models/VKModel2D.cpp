#include "VKModel2D.h"

#ifdef GRAPHICS_API_VULKAN

#define VK_NO_PROTOTYPES
#include <volk.h>

#include <cassert>

namespace kbb::vkApi::rendering::models
{
	VKModel2D::VKModel2D(
		VulkanDevice& vulkanDevice,
		const std::vector<vertex::Vertex2D>& verticies,
		const std::vector<unsigned int>& indices,
		DrawMode drawMode
	)
		: VKModel<vertex::Vertex2D>::VKModel(vulkanDevice, verticies, indices, drawMode) {}

	VKModel2D::~VKModel2D()
	{
	}

	void VKModel2D::bind(VkCommandBuffer commandBuffer)
	{
		VKModel<vertex::Vertex2D>::bind(commandBuffer);
	}

	void VKModel2D::draw(VkCommandBuffer commandBuffer)
	{
		VKModel<vertex::Vertex2D>::draw(commandBuffer);
	}
}
#endif