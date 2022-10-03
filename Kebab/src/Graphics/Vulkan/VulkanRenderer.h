#pragma once

#include <Kebab/Renderer/Renderer.h>

namespace kbb::renderer
{
	class VulkanRenderer : public Renderer
	{
	public:
		VulkanRenderer(const VulkanRenderer&) = delete;
		VulkanRenderer& operator=(const VulkanRenderer&) = delete;

		VulkanRenderer();

		void setClearColor(float r, float g, float b, float a) const override;

		void beginFrame() override;
		void endFrame() override;
	};
}
