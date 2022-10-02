#include "kbbpch.h"
#include "OpenGLProgram.h"

namespace kbb::renderer
{
	OpenGLProgram::OpenGLProgram(const char* filename1, const char* filename2)
		: OpenGLProgram(OpenGLShader(filename1), OpenGLShader(filename2))
	{}

	OpenGLProgram::OpenGLProgram(const char* filename1, const char* filename2, const char* filename3)
		: OpenGLProgram(OpenGLShader(filename1), OpenGLShader(filename2), OpenGLShader(filename3))
	{}

	OpenGLProgram::OpenGLProgram(const char* filename1, const char* filename2, const char* filename3, const char* filename4)
		: OpenGLProgram(OpenGLShader(filename1), OpenGLShader(filename2), OpenGLShader(filename3), OpenGLShader(filename4))
	{}
	
	OpenGLProgram::OpenGLProgram(const char* filename1, const char* filename2, const char* filename3, const char* filename4, const char* filename5)
		: OpenGLProgram(OpenGLShader(filename1), OpenGLShader(filename2), OpenGLShader(filename3), OpenGLShader(filename4), OpenGLShader(filename5))
	{}

	OpenGLProgram::OpenGLProgram(const OpenGLShader& s1, const OpenGLShader& s2)
		: m_handle(glCreateProgram())
	{
		attachShader(s1); attachShader(s2); 
		glLinkProgram(m_handle);
		printProgramInfoLog();
		detachShader(s1); detachShader(s2);
	}

	OpenGLProgram::OpenGLProgram(const OpenGLShader& s1, const OpenGLShader& s2, const OpenGLShader& s3)
		: m_handle(glCreateProgram())
	{
		attachShader(s1); attachShader(s2); attachShader(s3);
		glLinkProgram(m_handle);
		printProgramInfoLog();
		detachShader(s1); detachShader(s2); attachShader(s3);
	}

	OpenGLProgram::OpenGLProgram(const OpenGLShader& s1, const OpenGLShader& s2, const OpenGLShader& s3, const OpenGLShader& s4)
		: m_handle(glCreateProgram())
	{
		attachShader(s1); attachShader(s2); attachShader(s3); attachShader(s4);
		glLinkProgram(m_handle);
		printProgramInfoLog();
		detachShader(s1); detachShader(s2); attachShader(s3); detachShader(s4);
	}

	OpenGLProgram::OpenGLProgram(const OpenGLShader& s1, const OpenGLShader& s2, const OpenGLShader& s3, const OpenGLShader& s4, const OpenGLShader& s5)
		: m_handle(glCreateProgram())
	{
		attachShader(s1); attachShader(s2); attachShader(s3); attachShader(s4); attachShader(s5);
		glLinkProgram(m_handle);
		printProgramInfoLog();
		detachShader(s1); detachShader(s2); attachShader(s3); detachShader(s4); attachShader(s5);
	}

	void OpenGLProgram::use() const
	{
		glUseProgram(m_handle);
	}
	
	void OpenGLProgram::attachShader(const OpenGLShader& shader)
	{
		glAttachShader(m_handle, shader.getHandle());
	}

	void OpenGLProgram::detachShader(const OpenGLShader& shader)
	{
		glAttachShader(m_handle, shader.getHandle());
	}

	void OpenGLProgram::printProgramInfoLog()
	{
		char buffer[8192];
		GLsizei length = 0;
		glGetProgramInfoLog(m_handle, sizeof(buffer), &length, buffer);
		if (length)
		{
			KBB_CORE_ERROR("Shader program linking: {0}", buffer);
			assert(false);
		}
	}
}
