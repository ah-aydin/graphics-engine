#pragma once

#include "GLMesh.h"

#include <Engine/glApi/Rendering/Vertex/Vertex2D.h>

#ifdef GRAPHICS_API_OPENGL

namespace kbb::glApi
{
	class GLMesh2D: public GLMesh<Vertex2D>
	{
	public:
		GLMesh2D(
			const std::vector<Vertex2D>& verticies,
			const std::vector<uint32_t>& indices = std::vector<uint32_t>{},
			DrawMode drawMode = VERTEX_DRAW
		);
		~GLMesh2D();

		GLMesh2D(const GLMesh2D&) = delete;
		GLMesh2D& operator=(const GLMesh2D&) = delete;

		void draw();
	};
}

#endif
