#pragma once

#include <glm/glm.hpp>

#include "VertexAttribPointer.h"

#include <vector>

namespace kbb::glApi
{
	struct Vertex2D
	{
		glm::vec2 position{};
		glm::vec2 textureCoordinate{};

		static std::vector<VertexAttribPointer> getVertexAttribPointers();
	};
}
