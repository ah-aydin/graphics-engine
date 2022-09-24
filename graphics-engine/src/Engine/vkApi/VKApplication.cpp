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
			m_vulkanRenderer.render2D(commandBuffer, uniformBufferMemory, currentImageIndex, m_gameObjects2D);
			m_vulkanRenderer.render3D(commandBuffer, uniformBufferMemory, currentImageIndex, m_gameObjects3D);
			m_vulkanRenderer.endSwapchainRenderPass(commandBuffer);
			m_vulkanRenderer.endFrame(commandBuffer);
		}
		else
		{
			log_error_exception("Failed to get the buffers from begin frame");
		}

		glfwPollEvents();
		Time::tick();
	}

	vkDeviceWaitIdle(m_vulkanDevice.device());
	m_vulkanRenderer.destroyRenderSystems();
}

#include "GameObject/Primitives/Cube.h"
#include "GameObject/Primitives/Triangle.h"
void VKApplication::loadGameObjects()
{
	m_gameObjects2D.push_back(
		std::move(
			gameObject::primitives::triangles::triangleSierpinski2D(m_vulkanDevice)
		)
	);

	m_gameObjects2D.push_back(
		std::move(
			gameObject::primitives::triangles::triangle2D(m_vulkanDevice)
		)
	);

	m_gameObjects3D.push_back(
		std::move(
			gameObject::primitives::cubes::cube3D(m_vulkanDevice)
		)
	);
}

#endif