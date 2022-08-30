#include "GLWindow.h"

#include <glad/glad.h>

#include <Engine/Settings.h>

static void window_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    Settings::ratio = (float)width / height;
}

GLWindow::~GLWindow()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

/**
 * @brief Initializes the window
 *
 * @return true - Initialized with success | false - Initialization failed
 */
bool GLWindow::init()
{
    if (!BaseWindow::init()) return false;

    // Set callbacks
    glfwSetWindowSizeCallback(m_window, window_size_callback);

    // Create context
    glfwMakeContextCurrent(m_window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);
    glfwGetFramebufferSize(m_window, &m_width, &m_height);
    return true;
}

/**
 * Call this at the end of the main loop
 */
void GLWindow::tick()
{
    BaseWindow::tick();
    glfwSwapBuffers(m_window);
}

void GLWindow::setWindowHints()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}
