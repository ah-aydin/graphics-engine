#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

class VulkanWindow
{
public:
	VulkanWindow(int w, int h, std::string title);
	~VulkanWindow();

	VulkanWindow(const VulkanWindow&) = delete;
	VulkanWindow& operator=(const VulkanWindow&) = delete;

	inline bool shouldClose() { return glfwWindowShouldClose(m_window); }
	void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
	VkExtent2D getExtent() {
		return { static_cast<uint32_t>(m_width), static_cast<uint32_t>(m_height) };
	}

private:

	void initWindow();

	const int m_width;
	const int m_height;
	std::string m_title;

	GLFWwindow* m_window;
};

