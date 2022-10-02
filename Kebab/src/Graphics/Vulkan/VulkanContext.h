#pragma once

#include <Kebab/Renderer/GraphicsContext.h>

struct GLFWwindow;

namespace kbb::renderer
{
	class VulkanContext : public GraphicsContext
	{
	public:
		VulkanContext(GLFWwindow* windowHandle);
		
		void init() const override;
		void swapBuffers() const override;

	private:
		GLFWwindow* m_windowHandle;
	};
}
