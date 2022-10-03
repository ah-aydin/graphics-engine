#pragma once

#include <Kebab/Renderer/Buffers/IndexBuffer.h>

#include <glad/glad.h>

namespace kbb::renderer
{
	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(const OpenGLIndexBuffer&) = delete;
		OpenGLIndexBuffer& operator=(const OpenGLIndexBuffer&) = delete;

		OpenGLIndexBuffer(std::vector<uint32_t>& indices);
		~OpenGLIndexBuffer();

		void bind() const override;
		void unbind() const override;

	private:
		GLuint m_buffer;
	};
}