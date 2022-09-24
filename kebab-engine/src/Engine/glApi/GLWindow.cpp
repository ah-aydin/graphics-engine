#include "GLWindow.h"

#ifdef GRAPHICS_API_OPENGL

#include <Engine/Settings.h>
#include <Logging/Log.h>

void GLWindow::window_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    Settings::ratio = (float)width / height;
}


GLWindow::GLWindow(std::string title) : Window(title)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    m_window = glfwCreateWindow(WINDOW_SIZE, m_title.c_str(), WINDOW_MONITOR, NULL);
    if (!m_window)
    {
        log_error("ERROR::GLFW::WINDOW_CREATION");
        throw std::exception("Failed to create glfw window");
    }

    glfwSetWindowSizeCallback(m_window, window_size_callback);
    Window::setCallbacks();

    glfwMakeContextCurrent(m_window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);
    glfwGetFramebufferSize(m_window, &m_width, &m_height);

    Settings::ratio = (float)m_width / m_height;
}

GLWindow::~GLWindow()
{
}

#endif