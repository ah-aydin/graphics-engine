#pragma once

#ifdef GRAPHICS_API_VULKAN

#include "VKModel.h"

#include <Engine/vkApi/VkObjects/VulkanDevice.h>
#include <Engine/vkApi/Rendering/Verticies/Vertex2D.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

class VKModel2D : public VKModel<Vertex2D>
{
public:

	VKModel2D(
		VulkanDevice& vulkanDevice,
		const std::vector<Vertex2D>& verticies,
		const std::vector<unsigned int>& indices = std::vector<unsigned int>{},
		DrawMode drawMode = VERTEX_DRAW
	);
	~VKModel2D();

	VKModel2D(const VKModel2D &) = delete;
	VKModel2D &operator=(const VKModel2D &) = delete;

	void bind(VkCommandBuffer commandBuffer);
	void draw(VkCommandBuffer commandBuffer);
};

#endif