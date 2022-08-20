#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define WINDOW_TITLE "Graphics Engine"

#ifdef NDEBUG
// Open the window in full screen mode
#define WINDOW_MONITOR glfwGetPrimaryMonitor()
#define WINDOW_SIZE mode->width, mode->height
#else
// Open a window in windowed mode
#define WINDOW_MONITOR NULL
#define WINDOW_SIZE 1280, 720
#endif

/**
 * @brief A window class, there should only be one instance of this.
 *
 */
class GLWindow
{
public:
    GLWindow() {};
    ~GLWindow();

    bool init();

    // Call this on the update functino to switch screen buffers
    void tick();

    // Returns the width of the screen
    int getWidth() { return width; }
    // Returns the height of the screen
    int getHeight() { return height; }

    bool shouldClose() { return glfwWindowShouldClose(window); }
    void setShouldClose(bool val) { glfwSetWindowShouldClose(window, val); }

private:
    GLFWwindow* window = nullptr;

    int width = 800;
    int height = 600;
};