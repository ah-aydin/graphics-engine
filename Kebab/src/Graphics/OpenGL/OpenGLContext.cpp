#include "kbbpch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace kbb::renderer
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_windowHandle(windowHandle)
	{}

	void OpenGLContext::init() const
	{
		KBB_CORE_INFO("Initializing GLAD");
		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		if (!status)
		{
			KBB_CORE_ERROR("GLAD initialization failed");
		}
		KBB_CORE_INFO("\tOpenGL vendor: {0}", (const char*) glGetString(GL_VENDOR));
		KBB_CORE_INFO("\tOpenGL renderer: {0}", (const char*) glGetString(GL_RENDERER));
		KBB_CORE_INFO("\tOpenGL version: {0}", (const char*) glGetString(GL_VERSION));
	}

	void OpenGLContext::swapBuffers() const
	{
		glfwSwapBuffers(m_windowHandle);
	}

	void OpenGLContext::setClearColor(float r, float g, float b, float a) const
	{
		glClearColor(r, g, b, a);
	}

	void OpenGLContext::beginFrame() const
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void OpenGLContext::endFrame() const
	{

	}
}