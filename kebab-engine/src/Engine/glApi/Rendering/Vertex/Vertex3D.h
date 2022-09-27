#pragma once

#include <glm/glm.hpp>

#include "VertexAttribPointer.h"

#include <vector>

namespace kbb::glApi
{
	struct Vertex3D
	{
		glm::vec3 position{};
		glm::vec3 normal{};
		glm::vec2 textureCoordinate;

		static std::vector<VertexAttribPointer> getVertexAttribPointers();
	};
}
