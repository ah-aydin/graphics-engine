#include "kbbpch.h"
#include "RendererContext.h"

#ifdef GRAPHICS_API_OPENGL
#include <OpenGl/OpenGlContext.h>
#endif

namespace kbb
{
	RendererContext* RendererContext::s_rendererContext = nullptr;

	void RendererContext::Create(Window* window)
	{
#ifdef GRAPHICS_API_OPENGL
		s_rendererContext = new OpenGlContext(window->getHandle());
#endif
	}
}