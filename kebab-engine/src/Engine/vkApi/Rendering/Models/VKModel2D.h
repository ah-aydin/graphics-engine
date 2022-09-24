#pragma once

#ifdef GRAPHICS_API_VULKAN

#include "VKModel.h"

#include <Engine/vkApi/VulkanDevice.h>
#include <Engine/vkApi/Rendering/Vertex/Vertex2D.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace vkApi::rendering::models
{
	class VKModel2D : public VKModel<vertex::Vertex2D>
	{
	public:

		VKModel2D(
			VulkanDevice& vulkanDevice,
			const std::vector<vertex::Vertex2D>& verticies,
			const std::vector<unsigned int>& indices = std::vector<unsigned int>{},
			DrawMode drawMode = VERTEX_DRAW
		);
		~VKModel2D();

		VKModel2D(const VKModel2D &) = delete;
		VKModel2D &operator=(const VKModel2D &) = delete;

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);
	};
}
#endif