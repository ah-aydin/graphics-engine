#pragma once

#ifdef GRAPHICS_API_VULKAN

#include <Engine/vkApi/Rendering/Models/VKModel3D.h>

#include <memory>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

class VulkanDevice;

namespace kbb::vkApi::rendering::models::primitives::cubes
{
	std::shared_ptr<VKModel3D> createPosV3ColorV3(VulkanDevice& device, glm::vec3 offset);
}

#endif