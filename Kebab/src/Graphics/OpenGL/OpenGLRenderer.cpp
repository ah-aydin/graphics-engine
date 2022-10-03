#include "kbbpch.h"
#include "OpenGLRenderer.h"

#include <glad/glad.h>

namespace kbb::renderer
{
	OpenGLRenderer::OpenGLRenderer()
	{
	}

	void OpenGLRenderer::setClearColor(float r, float g, float b, float a) const
	{
		glClearColor(r, g, b, a);
	}

	void OpenGLRenderer::beginFrame()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void OpenGLRenderer::endFrame()
	{
	}
}
