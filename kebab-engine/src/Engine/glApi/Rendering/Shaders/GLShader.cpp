#include "GLShader.h"

#ifdef GRAPHICS_API_OPENGL

#include <assert.h>

#include <Common/Utils/fileUtils.h>
#include <Logging/Log.h>

#include "GLShaderUtils.h"

namespace kbb::glApi::rendering::shaders
{
	GLShader::GLShader(const char* fileName)
		: GLShader(GLShaderTypeFromFileName(fileName),
			readShaderFile(fileName).c_str(), fileName)
	{}

	GLShader::GLShader(GLenum type, const char* text, const char* debugFileName)
		: m_type(type), m_handle(glCreateShader(type))
	{
		glShaderSource(m_handle, 1, &text, nullptr);
		glCompileShader(m_handle);

		char buffer[8192];
		GLsizei length = 0;
		glGetShaderInfoLog(m_handle, sizeof(buffer), &length, buffer);

		if (length)
		{
			log_error("ERROR::SHADER::COMPILATOIN::%s (File %s)", buffer, debugFileName);
			printShaderSource(text);
			assert(false);
		}
	}

	GLShader::~GLShader()
	{
		glDeleteShader(m_handle);
	}
}

#endif