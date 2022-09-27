#include "Vertex2D.h"

namespace kbb::glApi
{
	std::vector<VertexAttribPointer> Vertex2D::getVertexAttribPointers()
	{
		return {
			// type, # of elements, offset
			{ GL_FLOAT, 2, offsetof(Vertex2D, position) },
			{ GL_FLOAT, 2, offsetof(Vertex2D, textureCoordinate) },
		};
	}
}
