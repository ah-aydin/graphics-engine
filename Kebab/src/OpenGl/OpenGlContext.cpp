#include "kbbpch.h"
#include "OpenGlContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace kbb
{
	OpenGlContext::OpenGlContext(GLFWwindow* window)
		: m_window(window)
	{}

	void OpenGlContext::init()
	{
		glfwMakeContextCurrent(m_window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		if (!status)
		{
			KBB_EXCEPTION("gladLoadGLLoader failed");
		}
	}

	void OpenGlContext::beginFrame()
	{
		glClearColor(1, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGlContext::endFrame()
	{
		glfwSwapBuffers(m_window);
	}
}
