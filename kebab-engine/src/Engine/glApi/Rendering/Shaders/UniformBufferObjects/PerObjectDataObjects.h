#pragma once

#include <glm/glm.hpp>

#ifdef GRAPHICS_API_OPENGL

namespace kbb::glApi
{
	struct PerObjectDataObject2D
	{
		glm::mat2 model;
		glm::vec2 offset;
	};

	struct PerObjectDataObject3D
	{
		alignas(16) glm::mat4 model;
	};
}
#endif
