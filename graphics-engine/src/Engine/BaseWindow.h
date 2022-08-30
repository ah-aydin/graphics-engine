#pragma once

#define GLFW_INCLUDE_NONE
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

class BaseWindow
{
protected:
	BaseWindow(const char* title);
	BaseWindow(const BaseWindow&) = delete;
	BaseWindow& operator=(BaseWindow&) = delete;
	virtual ~BaseWindow();

	virtual bool init();

	virtual void tick();

public:
	inline int getWidth() { return m_width; }
	inline int getHeight() { return m_height; }

public:
	inline bool shouldClose() { return glfwWindowShouldClose(m_window); }
	inline void setShouldClose(bool val) { glfwSetWindowShouldClose(m_window, val); }

protected:
	GLFWwindow* m_window = nullptr;
	virtual void setWindowHints() = 0;

	int m_width = 800;
	int m_height = 600;
	char* m_title;
};

