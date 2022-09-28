#pragma once

#ifdef GRAPHICS_API_VULKAN

#include <Engine/vkApi/GameObject/VKGameObject3D.h>

class VulkanDevice;

namespace kbb::vkApi::primitives::gameObject
{
	VKGameObject3D cube3D(VulkanDevice& vulkanDevice);
}

#endif