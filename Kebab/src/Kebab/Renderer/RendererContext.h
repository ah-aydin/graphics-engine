#pragma once

#include <Kebab/Window.h>

namespace kbb
{
	class RendererContext
	{
	protected:
		RendererContext() = default;
		
	public:
		virtual ~RendererContext() = default;

		virtual void init() = 0;
		virtual void beginFrame() = 0;
		virtual void endFrame() = 0;

		static void Init() { s_rendererContext->init(); }
		static void BeginFrame() { s_rendererContext->beginFrame(); };
		static void EndFrame() { s_rendererContext->endFrame(); };
		static void CleanUp() { delete s_rendererContext; }

	public:
		static void Create(Window* window);

	private:
		static RendererContext* s_rendererContext;
	};
}
