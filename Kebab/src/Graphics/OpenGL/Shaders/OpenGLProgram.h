#pragma once

#include "OpenGLShader.h"

namespace kbb::renderer
{
	class OpenGLProgram
	{
	public:
		OpenGLProgram(const char* filename1, const char* filename2);
		OpenGLProgram(const char* filename1, const char* filename2, const char* filename3);
		OpenGLProgram(const char* filename1, const char* filename2, const char* filename3, const char* filename4);
		OpenGLProgram(const char* filename1, const char* filename2, const char* filename3, const char* filename4, const char* filename5);

		OpenGLProgram(const OpenGLShader& s1, const OpenGLShader& s2);
		OpenGLProgram(const OpenGLShader& s1, const OpenGLShader& s2, const OpenGLShader& s3);
		OpenGLProgram(const OpenGLShader& s1, const OpenGLShader& s2, const OpenGLShader& s3, const OpenGLShader& s4);
		OpenGLProgram(const OpenGLShader& s1, const OpenGLShader& s2, const OpenGLShader& s3, const OpenGLShader& s4, const OpenGLShader& s5);

		void use() const;

		GLuint getHandle() const { return m_handle; }

	private:
		GLuint m_handle;

		void attachShader(const OpenGLShader& shader);
		void detachShader(const OpenGLShader& shader);

		void printProgramInfoLog();
	};
}
