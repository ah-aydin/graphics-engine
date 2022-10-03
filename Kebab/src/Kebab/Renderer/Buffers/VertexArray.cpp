#include "kbbpch.h"
#include "VertexArray.h"

#ifdef GRAPHICS_API_OPENGL
#include <Graphics/OpenGL/Buffers/OpenGLVertexArray.h>
#elif GRAPHICS_API_VULKAN
#include <Graphics/Vulkan/Buffers/VulkanVertexArray.h>
#else
error (No graphics api has been specified)
#endif

namespace kbb::renderer
{
	std::shared_ptr<VertexArray> VertexArray::Create(
		std::vector<float>& vertices,
		std::optional<VertexBufferLayout> layout,
		std::vector<uint32_t>& indices
	)
	{
#ifdef GRAPHICS_API_OPENGL
		KBB_CORE_ASSERTION(layout.has_value(), "OpenGL requires VertexBufferLayout on VertexArray creation");
		return std::make_shared<OpenGLVertexArray>(vertices, layout.value(), indices);
#elif GRAPHICS_API_VULKAN
		return std::make_shared<VulkanVertexArray>(vertices, indices);
#endif
	}
}
