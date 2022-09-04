#include "VKWindow.h"

#ifdef GRAPHICS_API_VULKAN

#include <Engine/Settings.h>
#include <Logging/Log.h>

#include "VKMacros.h"

VKWindow::VKWindow(std::string title) : Window(title)
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    m_window = glfwCreateWindow(WINDOW_SIZE, m_title.c_str(), WINDOW_MONITOR, NULL);
    if (!m_window)
    {
        log_error("ERROR::GLFW::WINDOW_CREATION");
        throw std::exception("Failed to create glfw window");
    }

    Window::setCallbacks();

    glfwGetFramebufferSize(m_window, &m_width, &m_height);
    Settings::ratio = (float)m_width / m_height;
}

VKWindow::~VKWindow()
{
}

void VKWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
{
    VK_CALL(glfwCreateWindowSurface(instance, m_window, nullptr, surface));
}

#endif