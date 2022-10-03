#include "kbbpch.h"
#include "IndexBuffer.h"

#ifdef GRAPHICS_API_OPENGL
#include <Graphics/OpenGL/Buffers/OpenGLIndexBuffer.h>
#elif GRAPHICS_API_VULKAN
#include <Graphics/Vulkan/Buffers/VulkanIndexBuffer.h>
#else
error (No graphics api has been specified)
#endif

namespace kbb::renderer
{
	std::shared_ptr<IndexBuffer> IndexBuffer::Create(std::vector<uint32_t>& indices)
	{
#ifdef GRAPHICS_API_OPENGL
		return std::make_shared<OpenGLIndexBuffer>(indices);
#elif GRAPHICS_API_VULKAN
		return std::make_shared<VulkanIndexBuffer>(indices);
#endif
	}

	IndexBuffer::IndexBuffer(uint32_t indexCount)
		: m_indexCount(indexCount)
	{}
}

