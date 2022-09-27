#include "Triangles.h"

#ifdef GRAPHICS_API_VULKAN

glm::vec3 leftColor = { 1, 0, 0 };
glm::vec3 rightColor = { 0, 1, 0 };
glm::vec3 centerColor = { 0, 0, 1 };
void sierpinski(std::vector<kbb::vkApi::Vertex2D> &verticies, int depth, glm::vec2 left, glm::vec2 right, glm::vec2 center)
{
	if (depth <= 0)
	{
		verticies.push_back({ left });
		verticies.push_back({ right });
		verticies.push_back({ center });
		return;
	}

	auto newRight = 0.5f * (right + center);
	auto newLeft = 0.5f * (left + center);
	auto newCenter = 0.5f * (left + right);
	sierpinski(verticies, depth - 1, left, newCenter, newLeft);
	sierpinski(verticies, depth - 1, newCenter, right, newRight);
	sierpinski(verticies, depth - 1, newLeft, newRight, center);
}

namespace kbb::vkApi::primitives::models
{
	std::unique_ptr<VKMesh2D> triangle2D(VulkanDevice& vulkanDevice)
	{
		std::vector<Vertex2D> verticiesTriangle{
			{{ -0.5f, 0.5f }}, {{ 0.5f, 0.5f }}, {{ 0.0f, -0.5f }}
		};
		std::vector<unsigned int> indicesTriangle{
			0, 1, 2
		};

		return std::make_unique<VKMesh2D>(vulkanDevice, verticiesTriangle, indicesTriangle, INDEX_DRAW);
	}
		
	std::unique_ptr<VKMesh2D> triangleSierpinski2D(VulkanDevice& vulkanDevice)
	{
		std::vector<Vertex2D> verticiesSierpinski{};
		sierpinski(verticiesSierpinski, 3, { -0.5f, 0.5f }, { 0.5f, 0.5f }, { 0.0f, -0.5f });
		return std::make_unique<VKMesh2D>(vulkanDevice, verticiesSierpinski);	
	}
}

#endif