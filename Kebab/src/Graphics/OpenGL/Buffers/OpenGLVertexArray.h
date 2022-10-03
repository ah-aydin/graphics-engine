#pragma once

#include <Kebab/Renderer/Buffers/VertexArray.h>

#include <Graphics/OpenGL/OpenGLCore.h>

#include "OpenGLVertexBuffer.h"
#include "OpenGLIndexBuffer.h"

namespace kbb::renderer
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray(const OpenGLVertexArray&) = delete;
		OpenGLVertexArray& operator=(const OpenGLVertexArray&) = delete;

		OpenGLVertexArray(std::vector<float>& vertices, VertexBufferLayout& layout, std::vector<uint32_t>& indices);
		~OpenGLVertexArray();

		void bind() const override;
		void unbind() const override;

		uint32_t getVertexCount() const override;
		uint32_t getIndexCount() const override;

	private:
		GLuint m_handle;

		std::shared_ptr<OpenGLVertexBuffer> m_vertexBuffer;
		std::shared_ptr<OpenGLIndexBuffer> m_indexBuffer;
	};
}
