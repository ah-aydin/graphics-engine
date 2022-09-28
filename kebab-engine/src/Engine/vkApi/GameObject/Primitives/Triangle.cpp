#include "Triangle.h"

#ifdef GRAPHICS_API_VULKAN

#include <Engine/vkApi/Rendering/Models/Primitives/Triangles.h>
#include "Triangle.h"
#include "Triangle.h"
#include "Triangle.h"

namespace kbb::vkApi::primitives::gameObject
{	
	VKGameObject2D triangleSierpinski2D(VulkanDevice& vulkanDevice)
	{
		std::shared_ptr<VKMesh2D> mesh = mesh::triangleSierpinski2D(vulkanDevice);

		auto sierpinksiGO = VKGameObject2D::createGameObject();
		sierpinksiGO.m_model = mesh;
		sierpinksiGO.m_color = { 0, 1, 0 };
		sierpinksiGO.m_transform.scale = { .5f, .5f };
		
		return sierpinksiGO;
	}

	VKGameObject2D triangle2D(VulkanDevice& vulkanDevice)
	{
		std::shared_ptr<VKMesh2D> mesh = mesh::triangle2D(vulkanDevice);

		auto triangle = VKGameObject2D::createGameObject();
		triangle.m_model = mesh;
		triangle.m_color = { 1, 0, 0 };
		triangle.m_transform.translation.x = 0.2f;
		triangle.m_transform.scale = { 2.f, 0.75f };
		triangle.m_transform.rotation = glm::radians(90.f);

		return triangle;
	}
}

#endif
