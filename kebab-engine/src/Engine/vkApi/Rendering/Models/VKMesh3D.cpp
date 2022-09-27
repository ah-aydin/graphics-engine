#include "VKMesh3D.h"

#ifdef GRAPHICS_API_VULKAN

#define VK_NO_PROTOTYPES
#include <volk.h>

#include <cassert>

namespace kbb::vkApi
{
	VKMesh3D::VKMesh3D(
		VulkanDevice& vulkanDevice,
		const std::vector<Vertex3D>& verticies,
		const std::vector<uint32_t>& indices,
		DrawMode drawMode
	)
		: VKMesh<Vertex3D>::VKMesh(vulkanDevice, verticies, indices, drawMode) {}

	VKMesh3D::~VKMesh3D()
	{
	}

	void VKMesh3D::bind(VkCommandBuffer commandBuffer)
	{
		VKMesh<Vertex3D>::bind(commandBuffer);
	}

	void VKMesh3D::draw(VkCommandBuffer commandBuffer)
	{
		VKMesh<Vertex3D>::draw(commandBuffer);
	}
}
#endif