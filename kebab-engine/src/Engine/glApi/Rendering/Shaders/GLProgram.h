#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Logging/Log.h>

#include "GLShader.h"

#include <cassert>
#include <map>

#ifdef GRAPHICS_API_OPENGL

namespace kbb::glApi
{
	template <typename PerFrameDataType, typename PerObjectDataType>
	class GLProgram
	{
	public:
		inline static GLuint PER_FRAME_DATA_BUFFER_INDEX = 0;
		inline static GLuint PER_OBJECT_DATA_BUFFER_INDEX = 1;
	public:
		// Constructers
		GLProgram(const GLShader& s1)
			: m_handle(glCreateProgram())
		{
			glAttachShader(m_handle, s1.getHandle());
			glLinkProgram(m_handle);
			printProgramInfoLog(m_handle);
			glDetachShader(m_handle, s1.getHandle());
			createUniformBuffers();
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
			createUniformBuffers();
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
			createUniformBuffers();
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
			createUniformBuffers();
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
			createUniformBuffers();
		}

		~GLProgram()
		{
			glDeleteBuffers(1, &m_perFrameDataBuffer);
			glDeleteBuffers(1, &m_perObjectDataBuffer);
			glDeleteProgram(m_handle);
		}

		GLProgram(const GLProgram&) = delete;
		GLProgram& operator=(const GLProgram&) = delete;

		void use() const
		{
			glUseProgram(m_handle);
			glBindBufferRange(GL_UNIFORM_BUFFER, GLProgram::PER_FRAME_DATA_BUFFER_INDEX, m_perFrameDataBuffer, 0, m_perFrameDataBufferSize);
			glBindBufferRange(GL_UNIFORM_BUFFER, GLProgram::PER_OBJECT_DATA_BUFFER_INDEX, m_perObjectDataBuffer, 0, m_perObjectDataBufferSize);
		}

		void setPerFrameData(const PerFrameDataType data)
		{
			glNamedBufferSubData(m_perFrameDataBuffer, 0, m_perFrameDataBufferSize, &data);
		}

		void setPerObjectData(const PerObjectDataType data)
		{
			glNamedBufferSubData(m_perObjectDataBuffer, 0, m_perObjectDataBufferSize, &data);
		}

		GLuint getHandle() const { return m_handle; }

		// Uniform setters
		void setInt(const char* uniformName, GLint v0)
		{
			glUniform1i(getUniformLocation(uniformName), v0);
		}

		void setFloat(const char* uniformName, GLfloat v0)
		{
			glUniform1f(getUniformLocation(uniformName), v0);
		}

		void setVector2f(const char* uniformName, GLfloat v0, GLfloat v1)
		{
			glUniform2f(getUniformLocation(uniformName), v0, v1);
		}
		void setVector2f(const char* uniformName, glm::vec2 vec)
		{
			setVector2f(uniformName, vec.x, vec.y);
		}

		void setVector3f(const char* uniformName, GLfloat v0, GLfloat v1, GLfloat v2)
		{
			glUniform3f(getUniformLocation(uniformName), v0, v1, v2);
		}
		void setVector3f(const char* uniformName, glm::vec3 vec)
		{
			setVector3f(uniformName, vec.x, vec.y, vec.z);
		}

		void setVector4f(const char* uniformName, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
		{
			glUniform4f(getUniformLocation(uniformName), v0, v1, v2, v3);
		}
		void setVector4f(const char* uniformName, glm::vec4 vec)
		{
			setVector4f(uniformName, vec.x, vec.y, vec.z, vec.w);
		}

		void setMatrix2f(
			const char* uniformName,
			GLfloat x0, GLfloat x1,
			GLfloat y0, GLfloat y1
		)
		{
			setMatrix2f(uniformName, glm::mat2(x0, y0, x1, y1));
		}
		void setMatrix2f(const char* uniformName, glm::mat2 mat)
		{
			glUniformMatrix2fv(getUniformLocation(uniformName), 0, GL_FALSE, glm::value_ptr(mat));
		}

		void setMatrix3f(
			const char* uniformName,
			GLfloat x0, GLfloat x1, GLfloat x2,
			GLfloat y0, GLfloat y1, GLfloat y2,
			GLfloat z0, GLfloat z1, GLfloat z2
		)
		{
			setMatrix3f(uniformName, glm::mat3(x0, y0, z0, x1, y1, z1, x2, y2, z2));
		}
		void setMatrix3f(const char* uniformName, glm::mat3 mat)
		{
			glUniformMatrix3fv(getUniformLocation(uniformName), 0, GL_FALSE, glm::value_ptr(mat));
		}

		void setMatrix4f(
			const char* uniformName,
			GLfloat x0, GLfloat x1, GLfloat x2, GLfloat x3,
			GLfloat y0, GLfloat y1, GLfloat y2, GLfloat y3,
			GLfloat z0, GLfloat z1, GLfloat z2, GLfloat z3,
			GLfloat w0, GLfloat w1, GLfloat w2, GLfloat w3
		)
		{
			setMatrix4f(uniformName, glm::mat4(x0, y0, z0, w0, x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3));
		}
		void setMatrix4f(const char* uniformName, glm::mat4 mat)
		{
			glUniformMatrix4fv(getUniformLocation(uniformName), 0, GL_FALSE, glm::value_ptr(mat));
		}

	private:
		GLuint m_perFrameDataBuffer, m_perObjectDataBuffer;
		GLsizeiptr m_perFrameDataBufferSize, m_perObjectDataBufferSize;
		void createUniformBuffers()
		{
			m_perFrameDataBufferSize = sizeof(PerFrameDataType);
			m_perObjectDataBufferSize = sizeof(PerObjectDataType);

			glCreateBuffers(1, &m_perFrameDataBuffer);
			glNamedBufferStorage(m_perFrameDataBuffer, m_perFrameDataBufferSize, nullptr, GL_DYNAMIC_STORAGE_BIT);

			glCreateBuffers(1, &m_perObjectDataBuffer);
			glNamedBufferStorage(m_perObjectDataBuffer, m_perObjectDataBufferSize, nullptr, GL_DYNAMIC_STORAGE_BIT);
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

		std::map<const char*, int> m_uniforms;
		GLint getUniformLocation(const char* uniformName)
		{
			std::map<const char*, int>::iterator it = m_uniforms.find(uniformName);
			if (it != m_uniforms.end())
			{
				return it->second;
			}

			GLint location = glGetUniformLocation(m_handle, uniformName);
			if (location == -1)
			{
				log_error_exception("ERROR::INVALID_UNIFORM_NAME::%s", uniformName);
			}
			m_uniforms.insert(std::pair<const char*, int>(uniformName, location));
			return location;
		}
	};
}

#endif