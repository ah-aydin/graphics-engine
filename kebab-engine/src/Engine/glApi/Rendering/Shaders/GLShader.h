#pragma once

#ifdef GRAPHICS_API_OPENGL

#include <glad/glad.h>

namespace kbb::glApi
{
	class GLShader
	{
	public:
		explicit GLShader(const char* fileName);
		GLShader(GLenum type, const char* text, const char* debugFileName = "");
		~GLShader();

		GLShader(const GLShader&) = delete;
		GLShader& operator=(const GLShader&) = delete;

		GLenum getType() const { return m_type; }
		GLuint getHandle() const { return m_handle; }

	private:
		GLenum m_type;
		GLuint m_handle;
	};
}
#endif