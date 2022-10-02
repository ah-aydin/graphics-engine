#pragma once

#include <Kebab/Renderer/GraphicsContext.h>

struct GLFWwindow;

namespace kbb::renderer
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		
		void init() override;
		void swapBuffers() const override;
		void terminate() override;

		void setClearColor(float r, float g, float b, float a) const override;

		void beginFrame() const override;
		void endFrame() const override;

	private:
		GLFWwindow* m_windowHandle;
	};
}
