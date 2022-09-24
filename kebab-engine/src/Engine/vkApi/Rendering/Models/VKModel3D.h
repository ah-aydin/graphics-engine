#pragma once

#ifdef GRAPHICS_API_VULKAN

#include "VKModel.h"

#include <Engine/vkApi/VulkanDevice.h>
#include <Engine/vkApi/Rendering/Vertex/Vertex3D.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace kbb::vkApi::rendering::models
{
	class VKModel3D : public VKModel<vertex::Vertex3D>
	{
	public:

		VKModel3D(
			VulkanDevice& vulkanDevice,
			const std::vector<vertex::Vertex3D>& verticies,
			const std::vector<unsigned int>& indices = std::vector<unsigned int>{},
			DrawMode drawMode = VERTEX_DRAW
		);
		~VKModel3D();

		VKModel3D(const VKModel3D&) = delete;
		VKModel3D& operator=(const VKModel3D&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);
	};
}
#endif