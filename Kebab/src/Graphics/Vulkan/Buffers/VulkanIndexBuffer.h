#pragma once

#include <Kebab/Renderer/IndexBuffer.h>

namespace kbb::renderer
{
	class VulkanIndexBuffer : public IndexBuffer
	{
	public:
		VulkanIndexBuffer(const VulkanIndexBuffer&) = delete;
		VulkanIndexBuffer operator=(const VulkanIndexBuffer&) = delete;

		VulkanIndexBuffer(std::vector<uint32_t>& indices);
		~VulkanIndexBuffer();

		void bind() const override;
		void unbind() const override;
	};
}
