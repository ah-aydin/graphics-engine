#pragma once

#include <GLFW/glfw3.h>
#include <string>

#ifdef NDEBUG
// Open the window in full screen mode
#define WINDOW_MONITOR glfwGetPrimaryMonitor()
#define WINDOW_SIZE mode->width, mode->height
#else
// Open a window in windowed mode
#define WINDOW_MONITOR NULL
#define WINDOW_SIZE 1280, 720
#endif

class Window
{
public:
	inline bool shouldClose() { return glfwWindowShouldClose(m_window); }
	inline void setShouldClose(bool val) { glfwSetWindowShouldClose(m_window, val); }

protected:
	Window(std::string title);
	virtual ~Window();

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

protected:
	std::string m_title;

	GLFWwindow* m_window = nullptr;
	int m_width = 800;
	int m_height = 600;

	void setCallbacks();

private:
	// Callbacks
	static void error_callback(int error, const char* description);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
};

