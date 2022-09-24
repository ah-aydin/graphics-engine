#pragma once

#ifdef GRAPHICS_API_VULKAN

#define GLFW_INCLUDE_VULKAN
#include <Engine/Window.h>

namespace vkApi
{
	class VKWindow : public Window
	{
	public:
		VKWindow(std::string title);
		~VKWindow();

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
		inline VkExtent2D getExtent() { return { static_cast<uint32_t>(m_width), static_cast<uint32_t>(m_height) }; }

		bool wasResized() { return m_framebufferResized; }
		void resetResizedFlag() { m_framebufferResized = false; }

	private:
		bool m_framebufferResized = false;
		static void window_resize_callback(GLFWwindow* window, int width, int height);
	};
}
#endif