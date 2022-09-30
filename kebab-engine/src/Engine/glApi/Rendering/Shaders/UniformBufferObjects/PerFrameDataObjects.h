#pragma once

#include <glm/glm.hpp>

#ifdef GRAPHICS_API_OPENGL

namespace kbb::glApi
{
	struct PerFrameDataObject2D
	{
		alignas(16) glm::mat4 proj;
		alignas(16) glm::mat4 view;
	};

	struct PerFrameDataObject3D
	{
		alignas(16) glm::mat4 proj;
		alignas(16) glm::mat4 view;
	};
}
#endif
