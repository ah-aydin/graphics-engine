#include "kbbpch.h"
#include "Renderer.h"

#ifdef GRAPHICS_API_OPENGL
#include <Graphics/OpenGL/OpenGLRenderer.h>
#elif GRAPHICS_API_VULKAN
#include <Graphics/Vulkan/VulkanRenderer.h>
#else
error (No graphics api has been specified)
#endif

namespace kbb::renderer
{
	std::unique_ptr<Renderer> Renderer::s_renderer;

	void Renderer::Create()
	{
#ifdef GRAPHICS_API_OPENGL
		s_renderer = std::make_unique<OpenGLRenderer>();
#elif GRAPHICS_API_VULKAN
		s_renderer = std::make_unique<VulkanRenderer>();
#endif
	}
}
