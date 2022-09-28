#pragma once

#include <glm/glm.hpp>

#include <Logging/Log.h>

namespace kbb::glApi
{
	struct PerFrameData2D
	{
		glm::mat2 transform{ 1.0f };
		glm::vec2 offset{ 0.0f, 0.0f };
	};
}
