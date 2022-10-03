#pragma once

namespace kbb::renderer
{
	class Renderer
	{
	protected:
		Renderer() = default;
	public:
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		virtual void setClearColor(float r, float g, float b, float a) const = 0;

		virtual void beginFrame() = 0;
		virtual void endFrame() = 0;

	public:
		static void SetClearColor(float r, float g, float b, float a) { s_renderer->setClearColor(r, g, b, a); }

		static void BeginFrame() { s_renderer->beginFrame(); }
		static void EndFrame() { s_renderer->endFrame(); }

	public:
		static void Create();

	private:
		static std::unique_ptr<Renderer> s_renderer;
	};
}
