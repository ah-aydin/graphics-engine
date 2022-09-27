#pragma once

#include "GLMesh.h"

#include <Engine/glApi/Rendering/Vertex/Vertex3D.h>

#ifdef GRAPHICS_API_OPENGL

namespace kbb::glApi
{
	class GLMesh3D: public GLMesh<Vertex3D>
	{
	public:
		GLMesh3D(
			const std::vector<Vertex3D>& verticies,
			const std::vector<uint32_t>& indices = std::vector<uint32_t>{},
			DrawMode drawMode = VERTEX_DRAW
		);
		~GLMesh3D();

		GLMesh3D(const GLMesh3D&) = delete;
		GLMesh3D& operator=(const GLMesh3D&) = delete;

		void draw();
	};
}

#endif
