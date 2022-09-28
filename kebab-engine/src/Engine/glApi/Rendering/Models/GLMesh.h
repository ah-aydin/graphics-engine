#pragma once


#include <glad/glad.h>

#include <Engine/Rendering/DrawMode.h>
#include <Engine/glApi/Rendering/Vertex/VertexAttribPointer.h>

#include <vector>

#ifdef GRAPHICS_API_OPENGL

namespace kbb::glApi
{
	template <typename VertexType>
	class GLMesh
	{
	protected:
		GLMesh(
			const std::vector<VertexType>& verticies,
			const std::vector<uint32_t>& indices= std::vector<uint32_t>{},
			DrawMode drawMode = VERTEX_DRAW
		)
			: m_vertexCount(static_cast<uint32_t>(verticies.size())), m_indexCount(static_cast<uint32_t>(indices.size())), m_drawMode(drawMode)
		{
			glCreateVertexArrays(1, &m_vao);
			glBindVertexArray(m_vao);

			createVertexBuffers(verticies);
			if (drawMode == INDEX_DRAW)
			{
				createIndexBuffers(indices);
			}

			glBindVertexArray(0);
		}

		virtual ~GLMesh()
		{
			glDeleteBuffers(1, &m_ibo);
			glDeleteBuffers(1, &m_vbo);
			glDeleteVertexArrays(1, &m_vao);
		}

		GLMesh(const GLMesh&) = delete;
		GLMesh& operator=(const GLMesh&) = delete;

	protected:
		void draw()
		{
			glBindVertexArray(m_vao);
			if (m_drawMode == VERTEX_DRAW)
			{
				glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
			}
			else if (m_drawMode == INDEX_DRAW)
			{
				glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
			}
			glBindVertexArray(0);
		}

	protected:
		void createVertexBuffers(const std::vector<VertexType>& verticies)
		{
			m_vertexCount = static_cast<uint32_t>(verticies.size());
			
			glCreateBuffers(1, &m_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(VertexType), verticies.data(), GL_STATIC_DRAW);

			std::vector<VertexAttribPointer> vertexAttribPointers = VertexType::getVertexAttribPointers();
			for (size_t i = 0; i < vertexAttribPointers.size(); ++i)
			{
				VertexAttribPointer vap = vertexAttribPointers[i];
				glEnableVertexAttribArray((GLuint) i);
				glVertexAttribPointer((GLuint)i, vap.count, vap.type, GL_FALSE, sizeof(VertexType), (void *)vap.offset);
			}
		}

		void createIndexBuffers(const std::vector<uint32_t>& indices)
		{
			m_indexCount = static_cast<uint32_t>(indices.size());

			glCreateBuffers(1, &m_ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
		}
		
	protected:
		GLuint m_vao = 0;
		GLuint m_vbo = 0;
		GLuint m_ibo = 0;
		uint32_t m_vertexCount;
		uint32_t m_indexCount;

		DrawMode m_drawMode;
	};
}

#endif
