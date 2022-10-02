#include "kbbpch.h"
#include "GraphicsContext.h"

#include <GLFW/glfw3.h>

#ifdef GRAPHICS_API_OPENGL
#include <Graphics/OpenGL/OpenGLContext.h>
#elif GRAPHICS_API_VULKAN
#include <Graphics/Vulkan/VulkanContext.h> 
#endif

namespace kbb::renderer
{
	std::unique_ptr<GraphicsContext> GraphicsContext::s_context;

	void GraphicsContext::Create(void* windowHandle)
	{
#ifdef GRAPHICS_API_OPENGL
		KBB_CORE_INFO("Creating OpenGLContext");
		s_context = std::make_unique<OpenGLContext>(static_cast<GLFWwindow*>(windowHandle));
#elif GRAPHICS_API_VULKAN
		KBB_CORE_INFO("Creating VulkanContext");
		s_context = std::make_unique<VulkanContext>(static_cast<GLFWWindow*>(windowHandle));
#endif
	}
}