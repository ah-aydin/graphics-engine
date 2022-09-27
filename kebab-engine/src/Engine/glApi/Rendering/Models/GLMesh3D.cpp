#include "GLMesh3D.h"

#ifdef GRAPHICS_API_OPENGL

namespace kbb::glApi
{
	GLMesh3D::GLMesh3D(
		const std::vector<Vertex3D>& verticies,
		const std::vector<uint32_t>& indices,
		DrawMode drawMode
	) : GLMesh<Vertex3D>::GLMesh(verticies, indices, drawMode) {}

	GLMesh3D::~GLMesh3D()
	{
	}

	void GLMesh3D::draw()
	{
		GLMesh<Vertex3D>::draw();
	}
}

#endif
