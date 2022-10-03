#pragma once

#include <Kebab/Core.h>

namespace kbb::renderer
{
	class GraphicsContext
	{
	protected:
		GraphicsContext() = default;
	public:
		GraphicsContext(const GraphicsContext&) = delete;
		GraphicsContext& operator=(const GraphicsContext&) = delete;

		// Implementation of the active graphics context
		virtual void init() = 0;
		virtual void swapBuffers() const = 0;
		virtual void terminate() = 0;
		
	public:
		// Interface to the active graphics context
		static void Init() { s_context->init(); }
		static void SwapBuffers() { s_context->swapBuffers(); }
		static void Terminate() { s_context->terminate(); }

	public:
		static void Create(void* windowHandle);

	private:
		static std::unique_ptr<GraphicsContext> s_context;
	};
}
