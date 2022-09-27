#pragma once

#ifdef GRAPHICS_API_VULKAN

#include "VKMesh.h"

#include <Engine/vkApi/VulkanDevice.h>
#include <Engine/vkApi/Rendering/Vertex/Vertex3D.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace kbb::vkApi
{
	class VKMesh3D : public VKMesh<Vertex3D>
	{
	public:

		VKMesh3D(
			VulkanDevice& vulkanDevice,
			const std::vector<Vertex3D>& verticies,
			const std::vector<uint32_t>& indices = std::vector<uint32_t>{},
			DrawMode drawMode = VERTEX_DRAW
		);
		~VKMesh3D();

		VKMesh3D(const VKMesh3D&) = delete;
		VKMesh3D& operator=(const VKMesh3D&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);
	};
}
#endif