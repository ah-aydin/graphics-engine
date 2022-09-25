#include "Triangle.h"

#ifdef GRAPHICS_API_VULKAN

#include <Engine/vkApi/Rendering/Models/Primitives/Triangles.h>

namespace kbb::vkApi::primitives::gameObjects
{	
	VKGameObject2D triangleSierpinski2D(VulkanDevice& vulkanDevice)
	{
		std::shared_ptr<VKModel2D> sierpinksiModel = models::triangleSierpinski2D(vulkanDevice);

		auto sierpinksiGO = VKGameObject2D::createGameObject();
		sierpinksiGO.m_model = sierpinksiModel;
		sierpinksiGO.m_color = { 0, 1, 0 };
		sierpinksiGO.m_transform.scale = { .5f, .5f };
		
		return sierpinksiGO;
	}

	VKGameObject2D triangle2D(VulkanDevice& vulkanDevice)
	{
		std::shared_ptr<VKModel2D> triangleModel = models::triangle2D(vulkanDevice);

		auto triangle = VKGameObject2D::createGameObject();
		triangle.m_model = triangleModel;
		triangle.m_color = { 1, 0, 0 };
		triangle.m_transform.translation.x = 0.2f;
		triangle.m_transform.scale = { 2.f, 0.75f };
		triangle.m_transform.rotation = glm::radians(90.f);

		return triangle;
	}
}

#endif