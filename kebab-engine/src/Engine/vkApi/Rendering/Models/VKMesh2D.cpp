#include "VKMesh2D.h"

#ifdef GRAPHICS_API_VULKAN

#define VK_NO_PROTOTYPES
#include <volk.h>

#include <cassert>

namespace kbb::vkApi
{
	VKMesh2D::VKMesh2D(
		VulkanDevice& vulkanDevice,
		const std::vector<Vertex2D>& verticies,
		const std::vector<uint32_t>& indices,
		DrawMode drawMode
	)
		: VKMesh<Vertex2D>::VKMesh(vulkanDevice, verticies, indices, drawMode) {}

	VKMesh2D::~VKMesh2D()
	{
	}

	void VKMesh2D::bind(VkCommandBuffer commandBuffer)
	{
		VKMesh<Vertex2D>::bind(commandBuffer);
	}

	void VKMesh2D::draw(VkCommandBuffer commandBuffer)
	{
		VKMesh<Vertex2D>::draw(commandBuffer);
	}
}
#endif