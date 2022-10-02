#pragma once

#include <Kebab/Core.h>

namespace kbb::renderer
{
	enum class VertexBufferType
	{
		None = 0,
		Float, Float2, Float3, Float4,
		MatFloat2, MatFloat3, MatFloat4
	};

	
	struct VertexBufferElement
	{
	public:
		VertexBufferType type;
		std::string name;

		// Number of elements
		uint32_t count;

		// Offset in bytes
		uint32_t offset;

		// Size in bytes
		uint32_t size;

		VertexBufferElement(VertexBufferType type, std::string name)
			: type(type), name(name), count(getBufferTypeCount(type)), size(getBufferTypeSize(type)), offset(0)
		{}
		
	private:
		uint32_t getBufferTypeCount(const VertexBufferType& type)
		{
			switch (type)
			{
			case VertexBufferType::Float:		return 1;
			case VertexBufferType::Float2:		return 2;
			case VertexBufferType::Float3:		return 3;
			case VertexBufferType::Float4:		return 4;
			case VertexBufferType::MatFloat2:	return 2 * 2;
			case VertexBufferType::MatFloat3:	return 3 * 3;
			case VertexBufferType::MatFloat4:	return 4 * 4;
			}

			KBB_CORE_ASSERTION(false, "Given type does not exist");
			return 0;
		}

		uint32_t getBufferTypeSize(const VertexBufferType& type)
		{
			switch (type)
			{
			case VertexBufferType::Float:		return 4 * getBufferTypeCount(type);
			case VertexBufferType::Float2:		return 4 * getBufferTypeCount(type);
			case VertexBufferType::Float3:		return 4 * getBufferTypeCount(type);
			case VertexBufferType::Float4:		return 4 * getBufferTypeCount(type);
			case VertexBufferType::MatFloat2:	return 4 * getBufferTypeCount(type);
			case VertexBufferType::MatFloat3:	return 4 * getBufferTypeCount(type);
			case VertexBufferType::MatFloat4:	return 4 * getBufferTypeCount(type);
			}

			KBB_CORE_ASSERTION(false, "Given type does not exist");
			return 0;
		}
	};

	class VertexBufferLayout
	{
	public:
		//VertexBufferLayout(const VertexBufferLayout&) = delete;
		VertexBufferLayout& operator=(const VertexBufferLayout&) = delete;
		
		VertexBufferLayout(std::vector<VertexBufferElement> elements);

		const VertexBufferElement& getElement(size_t index) const;

		// Number of elements
		size_t getCount() const { return m_elements.size(); }

		// Size in bytes
		uint32_t getSize() { return m_size; }

	private:
		std::vector<VertexBufferElement> m_elements;
		uint32_t m_size;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;

		VertexBuffer(uint32_t vertexCount);
		virtual ~VertexBuffer() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		int getVertexCount() { return m_vertexCount; }
		
	protected:
		uint32_t m_vertexCount;

	public:
		static std::shared_ptr<VertexBuffer> Create(std::vector<float>& vertices, VertexBufferLayout& layout);
	};
}
