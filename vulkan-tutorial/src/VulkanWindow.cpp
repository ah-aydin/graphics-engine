#include "VulkanWindow.h"

#include <stdexcept>

VulkanWindow::VulkanWindow(int w, int h, std::string title)
	:m_width(w), m_height(h), m_title(title)
{
	initWindow();
}

VulkanWindow::~VulkanWindow()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void VulkanWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
{
	if (glfwCreateWindowSurface(instance, m_window, nullptr, surface) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create window surface");
	}
}

void VulkanWindow::initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	
	m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
}
