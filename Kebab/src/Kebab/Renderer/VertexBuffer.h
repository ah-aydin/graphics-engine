#pragma once

#include <Kebab/Core.h>

#include <vector>

namespace kbb
{
	enum class VertexBufferElementType
	{
		None = 0, Float, Float2, Float3, Float4
	};

	static uint32_t vertexBufferElementSize(VertexBufferElementType type)
	{
		switch (type)
		{
		case VertexBufferElementType::Float:	return 4;
		case VertexBufferElementType::Float2:	return 4 * 2;
		case VertexBufferElementType::Float3:	return 4 * 3;
		case VertexBufferElementType::Float4:	return 4 * 4;
		default:
			KBB_EXCEPTION("Unknown vertex buffer element type");
			return 0;
		}
	}

	struct VertexBufferElement
	{
		VertexBufferElementType type;
		uint32_t offset;

		uint32_t getCount()
		{
			switch (type)
			{
			case VertexBufferElementType::Float:	return 1;
			case VertexBufferElementType::Float2:	return 2;
			case VertexBufferElementType::Float3:	return 3;
			case VertexBufferElementType::Float4:	return 4;
			}
		}

		uint32_t getSize()
		{
			return getCount() * vertexBufferElementSize(type);
		}
	};

	class VertexBufferLayout
	{
	public:
		VertexBufferLayout(std::initializer_list<VertexBufferElement> elements);

		const std::vector<VertexBufferElement> getElements() { return m_elements; }
		const uint32_t getStride() { return m_stride; }

	private:
		std::vector<VertexBufferElement> m_elements;
		uint32_t m_stride;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		static VertexBuffer* create(std::vector<float> vertices, std::optional<VertexBufferLayout> layout);
	};
}
