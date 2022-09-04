#pragma once

#ifdef GRAPHICS_API_VULKAN

#define GLFW_INCLUDE_VULKAN
#include <Engine/Common/Window.h>

class VKWindow : public Window
{
public:
	VKWindow(std::string title);
	~VKWindow();

	void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
	inline VkExtent2D getExtent() { return { static_cast<uint32_t>(m_width), static_cast<uint32_t>(m_height) }; }
};

#endif