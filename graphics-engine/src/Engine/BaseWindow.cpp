#include "BaseWindow.h"

#include <Logging/Log.h>
#include <Engine/Input.h>

static void error_callback(int error, const char* description)
{
    log_error("ERROR::GLFW::CALLBACK::%s", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        Input::keyPress(key);
    }
    if (action == GLFW_RELEASE) {
        Input::keyRelease(key);
    }
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    Input::mouseMotion(xpos, ypos);
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS) {
        Input::keyPress(button, true);
    }
    if (action == GLFW_RELEASE) {
        Input::keyRelease(button, true);
    }
}

BaseWindow::BaseWindow(const char* title)
    : m_title((char* )title)
{}

BaseWindow::~BaseWindow()
{}

bool BaseWindow::init()
{
    setWindowHints();
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    m_window = glfwCreateWindow(WINDOW_SIZE, m_title, WINDOW_MONITOR, NULL);    if (!m_window)
    {
        log_error("ERROR::GLFW::WINDOW_CREATION");
        return false;
    }

    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(m_window, key_callback);
    glfwSetCursorPosCallback(m_window, cursor_position_callback);
    glfwSetMouseButtonCallback(m_window, mouse_button_callback);
	return true;
}

void BaseWindow::tick()
{
    glfwPollEvents();
}