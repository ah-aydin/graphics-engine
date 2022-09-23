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
		VkDeviceMemory uniformBufferMemory;
		int currentImageIndex;
		m_vulkanRenderer.beginFrame(commandBuffer, uniformBufferMemory, currentImageIndex);
		if (commandBuffer && uniformBufferMemory)
		{
			m_vulkanRenderer.beginSwapchainRenderPass(commandBuffer);
			renderSystem2D.renderGameObjects2D(commandBuffer, m_gameObjects2D);
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

#include "GameObject/Templates/Cubes.h"
#include "GameObject/Templates/Triangles.h"
void VKApplication::loadGameObjects()
{
	std::shared_ptr<VKModel2D> sierpinksiModel = goModels::triangles::triangleSierpinski2D(m_vulkanDevice);
	auto sierpinksiGO = VKGameObject2D::createGameObject();
	sierpinksiGO.m_model = sierpinksiModel;
	sierpinksiGO.m_color = { 0, 1, 0 };
	sierpinksiGO.m_transform.scale = { .5f, .5f };
	m_gameObjects2D.push_back(std::move(sierpinksiGO));

	std::shared_ptr<VKModel2D> triangleModel = goModels::triangles::triangle2D(m_vulkanDevice);
	auto triangle = VKGameObject2D::createGameObject();
	triangle.m_model = triangleModel;
	triangle.m_color = { 1, 0, 0 };
	triangle.m_transform.translation.x = 0.2f;
	triangle.m_transform.scale = { 2.f, 0.75f };
	triangle.m_transform.rotation = glm::radians(90.f);

	m_gameObjects2D.push_back(std::move(triangle));

	std::shared_ptr<VKModel3D> cubeModel = goModels::cubes::createPosV3ColorV3(m_vulkanDevice, { 0, 0, 0 });
	auto cube = VKGameObject3D::createGameObject();
	cube.m_model = cubeModel;
	cube.m_transform.translation = { 0, 0, .5f };
	cube.m_transform.scale = { .5f, .5f, .5f };
	m_gameObjects3D.push_back(std::move(cube));
}

#endif