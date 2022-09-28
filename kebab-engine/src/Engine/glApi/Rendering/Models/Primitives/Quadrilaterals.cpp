#include "Quadrilaterlas.h"

namespace kbb::glApi::primitives::mesh
{
	std::shared_ptr<GLMesh2D> square()
	{
		std::vector<Vertex2D> verticies = {
			{{-1.0f, -1.0f}, {0.0f, 0.0f}},	// bottom-left
			{{-1.0f, 1.0f}, {0.0f, 1.0f}},	// top-left
			{{1.0f, 1.0f}, {1.0f, 1.0f}},	// top-right
			{{1.0f, -1.0f}, {1.0f, 0.0f}},	// bottom-right
		};
		std::vector<uint32_t> indices = {
			0, 3, 2,
			2, 1, 0
		};
		return std::make_shared<GLMesh2D>(verticies, indices, INDEX_DRAW);
	}
}