#pragma once

#include <Kebab/Renderer/Buffers/VertexBuffer.h>

#include <glad/glad.h>

namespace kbb::renderer
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(const OpenGLVertexBuffer&) = delete;
		OpenGLVertexBuffer& operator=(const OpenGLVertexBuffer&) = delete;

		OpenGLVertexBuffer(std::vector<float>& vertices, VertexBufferLayout& layout);
		~OpenGLVertexBuffer();

		void bind() const override;
		void unbind() const override;

	private:
		GLuint m_handle;
	};
}
