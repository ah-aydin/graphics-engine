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
		KBB_GL_CALL(glLinkProgram(m_handle));
		printProgramInfoLog();
		detachShader(s1); detachShader(s2);
	}

	OpenGLProgram::OpenGLProgram(const OpenGLShader& s1, const OpenGLShader& s2, const OpenGLShader& s3)
		: m_handle(glCreateProgram())
	{
		attachShader(s1); attachShader(s2); attachShader(s3);
		KBB_GL_CALL(glLinkProgram(m_handle));
		printProgramInfoLog();
		detachShader(s1); detachShader(s2); attachShader(s3);
	}

	OpenGLProgram::OpenGLProgram(const OpenGLShader& s1, const OpenGLShader& s2, const OpenGLShader& s3, const OpenGLShader& s4)
		: m_handle(glCreateProgram())
	{
		attachShader(s1); attachShader(s2); attachShader(s3); attachShader(s4);
		KBB_GL_CALL(glLinkProgram(m_handle));
		printProgramInfoLog();
		detachShader(s1); detachShader(s2); attachShader(s3); detachShader(s4);
	}

	OpenGLProgram::OpenGLProgram(const OpenGLShader& s1, const OpenGLShader& s2, const OpenGLShader& s3, const OpenGLShader& s4, const OpenGLShader& s5)
		: m_handle(glCreateProgram())
	{
		attachShader(s1); attachShader(s2); attachShader(s3); attachShader(s4); attachShader(s5);
		KBB_GL_CALL(glLinkProgram(m_handle));
		printProgramInfoLog();
		detachShader(s1); detachShader(s2); attachShader(s3); detachShader(s4); attachShader(s5);
	}

	void OpenGLProgram::use() const
	{
		KBB_GL_CALL(glUseProgram(m_handle));
	}
	
	void OpenGLProgram::attachShader(const OpenGLShader& shader)
	{
		KBB_GL_CALL(glAttachShader(m_handle, shader.getHandle()));
	}

	void OpenGLProgram::detachShader(const OpenGLShader& shader)
	{
		KBB_GL_CALL(glAttachShader(m_handle, shader.getHandle()));
	}

	void OpenGLProgram::printProgramInfoLog()
	{
		char buffer[8192];
		GLsizei length = 0;
		KBB_GL_CALL(glGetProgramInfoLog(m_handle, sizeof(buffer), &length, buffer));
		if (length)
		{
			KBB_CORE_ASSERTION(false, "Shader program linking: {0}", buffer);
		}
	}
}
