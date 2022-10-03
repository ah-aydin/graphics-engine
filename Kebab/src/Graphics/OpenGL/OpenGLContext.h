#pragma once

#include <Kebab/Renderer/GraphicsContext.h>

struct GLFWwindow;

namespace kbb::renderer
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(const OpenGLContext&) = delete;
		OpenGLContext& operator=(const OpenGLContext&) = delete;

		OpenGLContext(GLFWwindow* windowHandle);
		
		void init() override;
		void swapBuffers() const override;
		void terminate() override;
	private:
		GLFWwindow* m_windowHandle;
	};
}
