#pragma once

#ifdef GRAPHICS_API_VULKAN

#include <Engine/vkApi/GameObject/VKGameObject2D.h>

class VulkanDevice;

namespace kbb::vkApi::gameObject::primitives::triangles
{	
	VKGameObject2D triangleSierpinski2D(VulkanDevice& vulkanDevice);
	VKGameObject2D triangle2D(VulkanDevice& vulkanDevice);
}

#endif