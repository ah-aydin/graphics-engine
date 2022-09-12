#include "VKModel2D.h"

#ifdef GRAPHICS_API_VULKAN

#define VK_NO_PROTOTYPES
#include <volk.h>

#include <cassert>

VKModel2D::VKModel2D(VulkanDevice &vulkanDevice, const std::vector<Vertex2D>& verticies)
	: VKModel<Vertex2D>::VKModel(vulkanDevice, verticies) {}

VKModel2D::~VKModel2D()
{
}

void VKModel2D::bind(VkCommandBuffer commandBuffer)
{
	VKModel<Vertex2D>::bind(commandBuffer);
}

void VKModel2D::draw(VkCommandBuffer commandBuffer)
{
	VKModel<Vertex2D>::draw(commandBuffer);
}

#endif