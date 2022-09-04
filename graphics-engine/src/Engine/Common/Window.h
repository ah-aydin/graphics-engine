#pragma once

#include <string>
#include <GLFW/glfw3.h>

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
};

