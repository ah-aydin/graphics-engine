#include "kbbpch.h"
#include "OpenGLRenderer.h"

#include "OpenGLCore.h"

namespace kbb::renderer
{
	OpenGLRenderer::OpenGLRenderer()
	{
	}

	void OpenGLRenderer::setClearColor(float r, float g, float b, float a) const
	{
		KBB_GL_CALL(glClearColor(r, g, b, a));
	}

	void OpenGLRenderer::beginFrame()
	{
		KBB_GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
	}

	void OpenGLRenderer::endFrame()
	{
	}
}
