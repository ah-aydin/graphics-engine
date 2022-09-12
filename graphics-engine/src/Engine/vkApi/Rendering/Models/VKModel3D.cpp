#include "VKModel3D.h"

#ifdef GRAPHICS_API_VULKAN

#define VK_NO_PROTOTYPES
#include <volk.h>

#include <cassert>

VKModel3D::VKModel3D(VulkanDevice& vulkanDevice, const std::vector<Vertex3D>& verticies)
	: VKModel<Vertex3D>::VKModel(vulkanDevice, verticies) {}

VKModel3D::~VKModel3D()
{
}

void VKModel3D::bind(VkCommandBuffer commandBuffer)
{
	VKModel<Vertex3D>::bind(commandBuffer);
}

void VKModel3D::draw(VkCommandBuffer commandBuffer)
{
	VKModel<Vertex3D>::draw(commandBuffer);
}

#endif