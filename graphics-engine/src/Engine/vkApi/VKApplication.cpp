#include "VKApplication.h"

#ifdef GRAPHICS_API_VULKAN

#include "VKMacros.h"

#include "Rendering/Systems/BasicRenderSystem.h"

#include <Engine/Input.h>
#include <Engine/Time.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <stdexcept>

#define VK_NO_PROTOTYPES
#include <volk.h>

VKApplication::VKApplication() : Application()
{
	loadGameObjects();

	Input::createAction("QUIT", GLFW_KEY_ESCAPE, false);
}

VKApplication::~VKApplication()
{
}

void VKApplication::run()
{
	BasicRenderSystem renderSystem2D{
		m_vulkanDevice,
		m_vulkanRenderer.getSwapchainRenderPass(),
		"res/vulkan/basic/shader.vert2D.spv",
		"res/vulkan/basic/shader.frag2D.spv",
		m_vulkanRenderer.getUniformBuffers(),
		RENDER2D 
	};

	BasicRenderSystem renderSystem3D {
		m_vulkanDevice,
		m_vulkanRenderer.getSwapchainRenderPass(),
		"res/vulkan/basic/shader.vert3D.spv",
		"res/vulkan/basic/shader.frag3D.spv",
		m_vulkanRenderer.getUniformBuffers(),
		RENDER3D
	};

	while (!m_window.shouldClose())
	{
		if (Input::getAction("QUIT"))
		{
			m_window.setShouldClose(true);
		}
		
		VkCommandBuffer commandBuffer;
		VkBuffer uniformBuffer;
		VkDeviceMemory uniformBufferMemory;
		int currentImageIndex;
		m_vulkanRenderer.beginFrame(commandBuffer, uniformBuffer, uniformBufferMemory, currentImageIndex);
		if (commandBuffer && uniformBuffer && uniformBufferMemory)
		{
			m_vulkanRenderer.beginSwapchainRenderPass(commandBuffer);
			//renderSystem2D.renderGameObjects2D(commandBuffer, m_gameObjects2D);
			renderSystem3D.renderGameObjects3D(commandBuffer, uniformBufferMemory, currentImageIndex, m_gameObjects3D);
			m_vulkanRenderer.endSwapchainRenderPass(commandBuffer);
			m_vulkanRenderer.endFrame(commandBuffer);
		}
		else
		{
			throw std::runtime_error("Failed to get the buffers from begin frame");
		}

		glfwPollEvents();
		Time::tick();
	}

	vkDeviceWaitIdle(m_vulkanDevice.device());
}

glm::vec3 leftColor = { 1, 0, 0 };
glm::vec3 rightColor = { 0, 1, 0 };
glm::vec3 centerColor = { 0, 0, 1 };
void sierpinski(std::vector<Vertex2D> &verticies, int depth, glm::vec2 left, glm::vec2 right, glm::vec2 center)
{
	if (depth <= 0)
	{
		verticies.push_back({ left, leftColor });
		verticies.push_back({ right, rightColor });
		verticies.push_back({ center, centerColor });
		return;
	}

	auto newRight = 0.5f * (right + center);
	auto newLeft = 0.5f * (left + center);
	auto newCenter = 0.5f * (left + right);
	sierpinski(verticies, depth - 1, left, newCenter, newLeft);
	sierpinski(verticies, depth - 1, newCenter, right, newRight);
	sierpinski(verticies, depth - 1, newLeft, newRight, center);
}

std::unique_ptr<VKModel3D> createCubeModel(VulkanDevice& device, glm::vec3 offset) {
	std::vector<Vertex3D> vertices{

		// left face (white)
		{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
		{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
		{{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
		{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
		{{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
		{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

		// right face (yellow)
		{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
		{{.5f, .5f, .5f}, {.8f, .8f, .1f}},
		{{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
		{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
		{{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
		{{.5f, .5f, .5f}, {.8f, .8f, .1f}},

		// top face (orange, remember y axis points down)
		{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
		{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
		{{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
		{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
		{{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
		{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

		// bottom face (red)
		{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
		{{.5f, .5f, .5f}, {.8f, .1f, .1f}},
		{{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
		{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
		{{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
		{{.5f, .5f, .5f}, {.8f, .1f, .1f}},

		// nose face (blue)
		{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
		{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
		{{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
		{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
		{{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
		{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

		// tail face (green)
		{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
		{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
		{{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
		{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
		{{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
		{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

	};
	for (auto& v : vertices) {
		v.position += offset;
	}

	// These will not work properly with culling on
	// did not take into account the clockwise indexing
	std::vector<unsigned int> indices = {
		0, 1, 2,
		3, 4, 5,

		6,7,8,
		9,10,11,

		12,13,14,
		15,16,17,

		18,19,20,
		21,22,23,

		24,25,26,
		27,28,29,

		30,31,32,
		33,34,35,
	};

	return std::make_unique<VKModel3D>(device, vertices, indices, INDEX_DRAW);
}

void VKApplication::loadGameObjects()
{
	std::vector<Vertex2D> verticiesSierpinski{};
	sierpinski(verticiesSierpinski, 3, { -0.5f, 0.5f }, { 0.5f, 0.5f }, { 0.0f, -0.5f });
	auto sierpinksiModel = std::make_shared<VKModel2D>(m_vulkanDevice, verticiesSierpinski);
	auto sierpinksiGO = VKGameObject2D::createGameObject();
	sierpinksiGO.m_model = sierpinksiModel;
	sierpinksiGO.m_color = { 0, 1, 0 };
	sierpinksiGO.m_transform.scale = { .5f, .5f };
	m_gameObjects2D.push_back(std::move(sierpinksiGO));

	std::vector<Vertex2D> verticiesTriangle {
		{{ -0.5f, 0.5f }}, {{ 0.5f, 0.5f }}, {{ 0.0f, -0.5f }}
	};
	std::vector<unsigned int> indicesTriangle{
		0, 1, 2
	};
	auto triangleModel = std::make_shared<VKModel2D>(m_vulkanDevice, verticiesTriangle, indicesTriangle, INDEX_DRAW);

	auto triangle = VKGameObject2D::createGameObject();
	triangle.m_model = triangleModel;
	triangle.m_color = { 1, 0, 0 };
	triangle.m_transform.translation.x = 0.2f;
	triangle.m_transform.scale = { 2.f, 0.75f };
	triangle.m_transform.rotation = glm::radians(90.f);

	m_gameObjects2D.push_back(std::move(triangle));

	std::shared_ptr<VKModel3D> cubeModel = createCubeModel(m_vulkanDevice, { 0, 0, 0 });
	auto cube = VKGameObject3D::createGameObject();
	cube.m_model = cubeModel;
	cube.m_transform.translation = { 0, 0, .5f };
	cube.m_transform.scale = { .5f, .5f, .5f };
	m_gameObjects3D.push_back(std::move(cube));
}

#endif