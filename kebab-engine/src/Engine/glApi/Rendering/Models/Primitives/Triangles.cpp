#include "Triangles.h"

#ifdef GRAPHICS_API_OPENGL

namespace kbb::glApi::primitives::mesh
{
	std::shared_ptr<GLMesh2D> triangle2D()
	{
		std::vector<Vertex2D> verticies = {
			{{0.0f, 0.5f}, {0.5f, 1.0f}},
			{{0.5f, -0.5f}, {1.0f, 0.0f}},
			{{-0.5f, -0.5f}, {0.0f, 0.0f}}
		};
		std::vector<uint32_t> indices = { 0, 1, 2 };
		return std::make_shared<GLMesh2D>(verticies, indices, INDEX_DRAW);
	}
}

#endif
