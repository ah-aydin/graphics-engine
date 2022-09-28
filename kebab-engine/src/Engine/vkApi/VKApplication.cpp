#include "VKApplication.h"

#ifdef GRAPHICS_API_VULKAN

#include <Engine/Input.h>
#include <Engine/Time.h>

#include <Logging/Log.h>

#include "GameObject/Primitives/Cube.h"
#include "GameObject/Primitives/Triangle.h"

namespace kbb::vkApi
{
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
			}
			else
			{
				log_error_exception("Failed to get the buffers from begin frame");
			}
			m_vulkanRenderer.endFrame(commandBuffer);

			glfwPollEvents();
			Time::tick();
		}

		m_vulkanDevice.waitDeviceIdle();
		m_vulkanRenderer.destroyRenderSystems();
	}

	void VKApplication::loadGameObjects()
	{
		m_gameObjects2D.push_back(
			std::move(
				primitives::gameObject::triangleSierpinski2D(m_vulkanDevice)
			)
		);

		m_gameObjects2D.push_back(
			std::move(
				primitives::gameObject::triangle2D(m_vulkanDevice)
			)
		);

		m_gameObjects3D.push_back(
			std::move(
				primitives::gameObject::cube3D(m_vulkanDevice)
			)
		);
	}
}
#endif