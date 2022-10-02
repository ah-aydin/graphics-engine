#pragma once

#include <Kebab/Core.h>

namespace kbb::renderer
{
	class GraphicsContext
	{
	public:
		// Implementation of the active graphics context
		virtual void init() const = 0;
		virtual void swapBuffers() const = 0;
		
		virtual void setClearColor(float r, float g, float b, float a) const = 0;

		virtual void beginFrame() const = 0;
		virtual void endFrame() const = 0;

	public:
		// Interface to the active graphics context
		static void Init() { s_context->init(); }
		static void SwapBuffers() { s_context->swapBuffers(); }

		static void SetClearColor(float r, float g, float b, float a) { s_context->setClearColor(r, g, b, a); }

		static void BeginFrame() { s_context->beginFrame(); }
		static void EndFrame() { s_context->endFrame(); }

	public:
		static void Create(void* windowHandle);

	private:
		static std::unique_ptr<GraphicsContext> s_context;
	};
}
