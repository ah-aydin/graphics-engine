#pragma once

#include <glm/glm.hpp>

namespace kbb::glApi
{
	struct PerFrameData3D
	{
		glm::mat4 model{};
		glm::mat4 view{};
		glm::mat4 proj{};
	};
}
