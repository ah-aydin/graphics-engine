#pragma once

#include "VertexBuffer.h"

namespace kbb::renderer
{
	class VertexArray
	{
	public:
		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;

		VertexArray() {};
		virtual ~VertexArray() {};

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual uint32_t getVertexCount() const = 0;
		virtual uint32_t getIndexCount() const = 0;

	public:
		static std::shared_ptr<VertexArray> Create(
			std::vector<float>& vertices, 
			std::optional<VertexBufferLayout> layout,
			std::vector<uint32_t>& indices
		);
	};
}
