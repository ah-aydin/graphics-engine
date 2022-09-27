#pragma once

#ifdef GRAPHICS_API_VULKAN

#include <Engine/vkApi/Rendering/Models/VKMesh3D.h>

#include <memory>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

class VulkanDevice;

namespace kbb::vkApi::primitives::models
{
	std::shared_ptr<VKMesh3D> createPosV3ColorV3(VulkanDevice& device, glm::vec3 offset);
}

#endif