#pragma once

namespace kbb::renderer
{
	class IndexBuffer
	{
	public:
		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;

		IndexBuffer(uint32_t indexCount);
		virtual ~IndexBuffer() {};

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		int getIndexCount() { return m_indexCount; }

	protected:
		uint32_t m_indexCount;

	public:
		static std::shared_ptr<IndexBuffer> Create(std::vector<uint32_t>& indices);
	};
}
