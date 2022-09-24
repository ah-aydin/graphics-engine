#pragma once

#ifdef GRAPHICS_API_OPENGL

#include <glad/glad.h>

#include <Engine/Window.h>

namespace kbb::glApi
{
	class GLWindow : public Window
	{
	public:
		GLWindow(std::string title);
		~GLWindow();

		inline void swapBuffer() { glfwSwapBuffers(m_window); }

	private:
		static void window_size_callback(GLFWwindow* window, int width, int height);
	};
}
#endif