#pragma once

#include <Kebab/Renderer/Renderer.h>

namespace kbb::renderer
{
	class OpenGLRenderer : public Renderer
	{
	public:
		OpenGLRenderer(const OpenGLRenderer&) = delete;
		OpenGLRenderer& operator=(const OpenGLRenderer&) = delete;

		OpenGLRenderer();

		void setClearColor(float r, float g, float b, float a) const override;

		void beginFrame() override;
		void endFrame() override;
	};
}
