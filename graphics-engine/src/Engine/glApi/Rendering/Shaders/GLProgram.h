#pragma once
#include "GLShader.h"

#ifdef GRAPHICS_API_OPENGL

class GLProgram
{
public:
	GLProgram(const GLShader& s1);
	GLProgram(const GLShader& s1, const GLShader& s2);
	GLProgram(const GLShader& s1, const GLShader& s2, const GLShader& s3);
	GLProgram(const GLShader& s1, const GLShader& s2, const GLShader& s3, const GLShader& s4, const GLShader& s5);
	~GLProgram();

	void use() const;
	GLuint getHandle() const { return m_handle; }

private:
	GLuint m_handle;
};


#endif