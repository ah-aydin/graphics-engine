#pragma once

#ifdef GRAPHICS_API_VULKAN

#include "VKMesh.h"

#include <Engine/vkApi/VulkanDevice.h>
#include <Engine/vkApi/Rendering/Vertex/Vertex2D.h>

#include <vector>

namespace kbb::vkApi
{
	class VKMesh2D : public VKMesh<Vertex2D>
	{
	public:

		VKMesh2D(
			VulkanDevice& vulkanDevice,
			const std::vector<Vertex2D>& verticies,
			const std::vector<uint32_t>& indices = std::vector<unsigned int>{},
			DrawMode drawMode = VERTEX_DRAW
		);
		~VKMesh2D();

		VKMesh2D(const VKMesh2D &) = delete;
		VKMesh2D &operator=(const VKMesh2D &) = delete;

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);
	};
}
#endif