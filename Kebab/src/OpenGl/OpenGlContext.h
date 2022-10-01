#pragma once

#include "Kebab/Renderer/RendererContext.h"

struct GLFWwindow;

namespace kbb
{
	class OpenGlContext : public RendererContext
	{
	public:
		OpenGlContext(GLFWwindow* window);

		void init() override;
		void beginFrame() override;
		void endFrame() override;

	private:
		GLFWwindow* m_window;
	};
}
