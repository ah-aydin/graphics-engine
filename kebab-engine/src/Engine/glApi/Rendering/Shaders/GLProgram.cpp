#include "GLProgram.h"

#ifdef GRAPHICS_API_OPENGL

#include <assert.h>

#include <Logging/Log.h>

void printProgramInfoLog(GLuint handle)
{
	char buffer[8192];
	GLsizei length = 0;
	glGetProgramInfoLog(handle, sizeof(buffer), &length, buffer);
	if (length)
	{
		log_error("ERROR::SHADER_PROGRAM::LINKING::%s\n", buffer);
		assert(false);
	}
}

GLProgram::GLProgram(const GLShader& s1) 
	: m_handle(glCreateProgram())
{
	glAttachShader(m_handle, s1.getHandle());
	glLinkProgram(m_handle);
	printProgramInfoLog(m_handle);
}

GLProgram::GLProgram(const GLShader& s1, const GLShader& s2) 
	: m_handle(glCreateProgram())
{
	glAttachShader(m_handle, s1.getHandle());
	glAttachShader(m_handle, s2.getHandle());
	glLinkProgram(m_handle);
	printProgramInfoLog(m_handle);
}

GLProgram::GLProgram(const GLShader& s1, const GLShader& s2, const GLShader& s3) 
	: m_handle(glCreateProgram())
{
	glAttachShader(m_handle, s1.getHandle());
	glAttachShader(m_handle, s2.getHandle());
	glAttachShader(m_handle, s3.getHandle());
	glLinkProgram(m_handle);
	printProgramInfoLog(m_handle);
}

GLProgram::GLProgram(const GLShader& s1, const GLShader& s2, const GLShader& s3, const GLShader& s4, const GLShader& s5) 
	: m_handle(glCreateProgram())
{
	glAttachShader(m_handle, s1.getHandle());
	glAttachShader(m_handle, s2.getHandle());
	glAttachShader(m_handle, s3.getHandle());
	glAttachShader(m_handle, s4.getHandle());
	glAttachShader(m_handle, s5.getHandle());
	glLinkProgram(m_handle);
	printProgramInfoLog(m_handle);
}

GLProgram::~GLProgram()
{
	glDeleteProgram(m_handle);
}

void GLProgram::use() const
{
	glUseProgram(m_handle);
}

#endif