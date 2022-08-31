#include "VKWindow.h"

#ifdef GRAPHICS_API_VULKAN

#include <Engine/Settings.h>

static void window_size_callback(GLFWwindow* window, int width, int height)
{
	Settings::ratio = (float)width / height;
}

bool VKWindow::init()
{
	if (!BaseWindow::init()) return false;
	// Set callbacks
	//glfwSetWindowSizeCallback(m_window, window_size_callback)
	return true;
}

void VKWindow::tick()
{
	BaseWindow::tick();
}

void VKWindow::setWindowHints()
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}

#endif