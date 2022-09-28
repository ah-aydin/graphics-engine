#pragma once

#include <Logging/Log.h>

#include "GLShader.h"

#include <cassert>

#ifdef GRAPHICS_API_OPENGL

namespace kbb::glApi
{
	template <typename PerFrameDataType>
	class GLProgram
	{
	public:
		inline static GLuint PER_FRAME_DATA_INDEX = 0;
	public:
		GLProgram(const GLShader& s1)
			: m_handle(glCreateProgram())
		{
			glAttachShader(m_handle, s1.getHandle());
			glLinkProgram(m_handle);
			printProgramInfoLog(m_handle);
			glDetachShader(m_handle, s1.getHandle());
			createPerFrameDataBuffer();
		}

		GLProgram(const GLShader& s1, const GLShader& s2)
			: m_handle(glCreateProgram())
		{
			glAttachShader(m_handle, s1.getHandle());
			glAttachShader(m_handle, s2.getHandle());
			glLinkProgram(m_handle);
			printProgramInfoLog(m_handle);
			glDetachShader(m_handle, s1.getHandle());
			glDetachShader(m_handle, s2.getHandle());
			createPerFrameDataBuffer();
		}
		
		GLProgram(const GLShader& s1, const GLShader& s2, const GLShader& s3)
			: m_handle(glCreateProgram())
		{
			glAttachShader(m_handle, s1.getHandle());
			glAttachShader(m_handle, s2.getHandle());
			glAttachShader(m_handle, s3.getHandle());
			glLinkProgram(m_handle);
			printProgramInfoLog(m_handle);
			glDetachShader(m_handle, s1.getHandle());
			glDetachShader(m_handle, s2.getHandle());
			glDetachShader(m_handle, s3.getHandle());
			createPerFrameDataBuffer();
		}

		GLProgram(const GLShader& s1, const GLShader& s2, const GLShader& s3, const GLShader& s4)
			: m_handle(glCreateProgram())
		{
			glAttachShader(m_handle, s1.getHandle());
			glAttachShader(m_handle, s2.getHandle());
			glAttachShader(m_handle, s3.getHandle());
			glAttachShader(m_handle, s4.getHandle());
			printProgramInfoLog(m_handle);
			glDetachShader(m_handle, s1.getHandle());
			glDetachShader(m_handle, s2.getHandle());
			glDetachShader(m_handle, s3.getHandle());
			glDetachShader(m_handle, s4.getHandle());
			createPerFrameDataBuffer();
		}

		GLProgram(const GLShader& s1, const GLShader& s2, const GLShader& s3, const GLShader& s4, const GLShader& s5)
			: m_handle(glCreateProgram())
		{
			glAttachShader(m_handle, s1.getHandle());
			glAttachShader(m_handle, s2.getHandle());
			glAttachShader(m_handle, s3.getHandle());
			glAttachShader(m_handle, s4.getHandle());
			glAttachShader(m_handle, s5.getHandle());
			glLinkProgram(m_handle);
			printProgramInfoLog(m_handle);
			glDetachShader(m_handle, s1.getHandle());
			glDetachShader(m_handle, s2.getHandle());
			glDetachShader(m_handle, s3.getHandle());
			glDetachShader(m_handle, s4.getHandle());
			glDetachShader(m_handle, s5.getHandle());
			createPerFrameDataBuffer();
		}

		~GLProgram()
		{
			glDeleteBuffers(1, &m_perFrameDataBuffer);
			glDeleteProgram(m_handle);
		}

		GLProgram(const GLProgram&) = delete;
		GLProgram& operator=(const GLProgram&) = delete;

		void use() const
		{
			glUseProgram(m_handle);
			glBindBufferRange(GL_UNIFORM_BUFFER, GLProgram::PER_FRAME_DATA_INDEX, m_perFrameDataBuffer, 0, m_perFrameDataBufferSize);
		}

		void setPerFrameData(const PerFrameDataType data)
		{
			glNamedBufferSubData(m_perFrameDataBuffer, 0, m_perFrameDataBufferSize, &data);
		}

		void setMeh(GLuint buffer, const PerFrameDataType data)
		{
			glNamedBufferSubData(buffer, 0, m_perFrameDataBufferSize, &data);
		}

		GLuint getHandle() const { return m_handle; }

	private:
		void createPerFrameDataBuffer()
		{
			m_perFrameDataBufferSize = sizeof(PerFrameDataType);

			glCreateBuffers(1, &m_perFrameDataBuffer);
			glNamedBufferStorage(m_perFrameDataBuffer, m_perFrameDataBufferSize, nullptr, GL_DYNAMIC_STORAGE_BIT);
		}

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

		GLuint m_handle;
		GLuint m_perFrameDataBuffer;
		GLsizeiptr m_perFrameDataBufferSize;
	};
}

#endif