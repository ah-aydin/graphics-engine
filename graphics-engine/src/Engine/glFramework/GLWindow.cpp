#include "GLWindow.h"

#include <glad/glad.h>

#include <Engine/Input.h>
#include <Logging/Log.h>

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

GLWindow::~GLWindow()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

/**
 * @brief Initializes the window
 *
 * @return true - Initialized with success | false - Initialization failed
 */
bool GLWindow::init()
{
    // Create window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    window = glfwCreateWindow(WINDOW_SIZE, WINDOW_TITLE, WINDOW_MONITOR, NULL);
    if (!window)
    {
        log_error("ERROR::GLFW::WINDOW_CREATION");
        return false;
    }

    // Set callbacks
    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // Create context
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);
    glfwGetFramebufferSize(window, &width, &height);
    return true;
}

/**
 * Call this at the end of the main loop
 */
void GLWindow::tick()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}