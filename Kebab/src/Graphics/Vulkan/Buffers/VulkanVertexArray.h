#pragma once

#include <Kebab/Renderer/VertexArray.h>

#include "VulkanVertexBuffer.h"
#include "VulkanIndexBuffer.h"

namespace kbb::renderer
{
	class VulkanVertexArray : public VertexArray
	{
	public:
		VulkanVertexArray(const VulkanVertexArray&) = delete;
		VulkanVertexArray& operator=(const VulkanVertexArray&) = delete;

		VulkanVertexArray(std::vector<float>& vertices, std::vector<uint32_t> indices);
		~VulkanVertexArray();

		void bind() const override;
		void unbind() const override;

		uint32_t getVertexCount() const override;
		uint32_t getIndexCount() const override;

	private:
		std::shared_ptr<VulkanVertexBuffer> m_vertexBuffer;
		std::shared_ptr<VulkanIndexBuffer> m_indexBuffer;
	};
}
