#pragma once

#include <Kebab/Renderer/Buffers/VertexBuffer.h>

namespace kbb::renderer
{
	class VulkanVertexBuffer : public VertexBuffer
	{
	public:
		VulkanVertexBuffer(const VulkanVertexBuffer&) = delete;
		VulkanVertexBuffer& operator=(const VulkanVertexBuffer&) = delete;

		VulkanVertexBuffer(std::vector<float>& vertices);
		~VulkanVertexBuffer();

		void bind() const override;
		void unbind() const override;
	};
}
