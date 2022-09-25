#pragma once

#ifdef GRAPHICS_API_VULKAN

#define VK_NO_PROTOTYPES
#include <Engine/Application.h>

#include "VKWindow.h"
#include "VulkanDevice.h"
#include "Rendering/VulkanRenderer.h"
#include "GameObject/VKGameObject2D.h"
#include "GameObject/VKGameObject3D.h"

#include <vector>
#include <memory>

namespace kbb::vkApi
{
	class VKApplication : public Application
	{
	public:
		VKApplication();
		~VKApplication();

		void run() override;

	private:

		VKWindow m_window{ "Vulkan Application" };
		VulkanDevice m_vulkanDevice{ m_window };
		VulkanRenderer m_vulkanRenderer{ m_window, m_vulkanDevice };

		void loadGameObjects();
		std::vector<VKGameObject2D> m_gameObjects2D;
		std::vector<VKGameObject3D> m_gameObjects3D;
	};
}
#endif