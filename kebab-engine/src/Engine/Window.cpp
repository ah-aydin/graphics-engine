#include "Window.h"

#include <Logging/Log.h>
#include <Engine/Input.h>

namespace kbb
{
    void Window::error_callback(int error, const char* description)
    {
        log_error("ERROR::GLFW::CALLBACK::%s", description);
    }

    void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (action == GLFW_PRESS) {
            Input::keyPress(key);
        }
        if (action == GLFW_RELEASE) {
            Input::keyRelease(key);
        }
    }

    void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
        Input::mouseMotion(xpos, ypos);
    }

    void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        if (action == GLFW_PRESS) {
            Input::keyPress(button, true);
        }
        if (action == GLFW_RELEASE) {
            Input::keyRelease(button, true);
        }
    }

    Window::Window(std::string title) : m_title(title)
    {
        if (glfwInit() == GLFW_FALSE)
        {
            log_error("ERROR::GLFW::INITIALIZATION");
            throw std::exception("glfw initalization error");
        }
    }

    Window::~Window()
    {
        glfwTerminate();
    }

    void Window::setCallbacks()
    {
        glfwSetErrorCallback(error_callback);
        glfwSetKeyCallback(m_window, key_callback);
        glfwSetCursorPosCallback(m_window, cursor_position_callback);
        glfwSetMouseButtonCallback(m_window, mouse_button_callback);
    }
}
