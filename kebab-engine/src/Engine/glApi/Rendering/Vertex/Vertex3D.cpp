#include "Vertex3D.h"

namespace kbb::glApi
{
	std::vector<VertexAttribPointer> Vertex3D::getVertexAttribPointers()
	{
		return {
			// type, # of elements, offset
			{ GL_FLOAT, 3, offsetof(Vertex3D, position) },
			{ GL_FLOAT, 3, offsetof(Vertex3D, normal) },
			{ GL_FLOAT, 2, offsetof(Vertex3D, textureCoordinate) }
		};
	}
}