#include "GLMesh2D.h"

#ifdef GRAPHICS_API_OPENGL

namespace kbb::glApi
{
	GLMesh2D::GLMesh2D(
		const std::vector<Vertex2D>& verticies,
		const std::vector<uint32_t>& indices,
		DrawMode drawMode
	) : GLMesh<Vertex2D>::GLMesh(verticies, indices, drawMode) {}

	GLMesh2D::~GLMesh2D()
	{
	}

	void GLMesh2D::draw()
	{
		GLMesh<Vertex2D>::draw();
	}
}

#endif