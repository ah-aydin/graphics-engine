#pragma once

#ifdef GRAPHICS_API_VULKAN

#include <Engine/vkApi/Rendering/Models/VKModel2D.h>

#include <memory>

class VulkanDevice;

namespace kbb::vkApi::rendering::models::primitives::triangles
{
	std::unique_ptr<VKModel2D> triangle2D(VulkanDevice& vulkanDevice);
	std::unique_ptr<VKModel2D> triangleSierpinski2D(VulkanDevice& vulkanDevice);
}

#endif
