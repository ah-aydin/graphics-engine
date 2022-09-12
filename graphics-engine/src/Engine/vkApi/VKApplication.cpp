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
		RENDER2D 
	};

	while (!m_window.shouldClose())
	{
		if (Input::getAction("QUIT"))
		{
			m_window.setShouldClose(true);
		}

		if (auto commandBuffer = m_vulkanRenderer.beginFrame())
		{
			m_vulkanRenderer.beginSwapchainRenderPass(commandBuffer);
			renderSystem2D.renderGameObjects(commandBuffer, m_gameObjects);
			m_vulkanRenderer.endSwapchainRenderPass(commandBuffer);
			m_vulkanRenderer.endFrame(commandBuffer);
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

void VKApplication::loadGameObjects()
{
	std::vector<Vertex2D> verticiesSierpinski{};
	sierpinski(verticiesSierpinski, 3, { -0.5f, 0.5f }, { 0.5f, 0.5f }, { 0.0f, -0.5f });
	std::vector<Vertex2D> verticiesTriangle {
		{{ -0.5f, 0.5f }}, {{ 0.5f, 0.5f }}, {{ 0.0f, -0.5f }}
	};
	auto m_model = std::make_shared<VKModel2D>(m_vulkanDevice, verticiesTriangle);

	auto triangle = VKGameObject2D::createGameObject();
	triangle.m_model = m_model;
	triangle.m_color = { 1, 0, 0 };
	triangle.m_transform.translation.x = 0.2f;
	triangle.m_transform.scale = { 2.f, 0.75f };
	triangle.m_transform.rotation = glm::radians(90.f);

	m_gameObjects.push_back(std::move(triangle));
}

#endif