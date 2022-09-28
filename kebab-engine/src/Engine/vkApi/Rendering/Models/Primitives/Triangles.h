#pragma once

#ifdef GRAPHICS_API_VULKAN

#include <Engine/vkApi/Rendering/Models/VKMesh2D.h>

#include <memory>

class VulkanDevice;

namespace kbb::vkApi::primitives::mesh
{
	std::unique_ptr<VKMesh2D> triangle2D(VulkanDevice& vulkanDevice);
	std::unique_ptr<VKMesh2D> triangleSierpinski2D(VulkanDevice& vulkanDevice);
}

#endif
