#pragma once

#include <vector>

namespace kbb
{
	class IndexBuffer
	{
	protected:
		IndexBuffer(size_t indexCount);
	public:
		virtual ~IndexBuffer() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		uint32_t getCount() { return m_indexCount; }

		static IndexBuffer* create(std::vector<uint32_t> indices);
	protected:
		uint32_t m_indexCount;
	};
}
