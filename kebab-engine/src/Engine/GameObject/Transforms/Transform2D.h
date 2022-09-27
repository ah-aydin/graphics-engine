#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace kbb
{
	struct Transform2D
	{
		glm::vec2 translation{};
		glm::vec2 scale{ 1.0f, 1.0f };
		float rotation = 0.f;

		glm::mat2 mat2() {
			glm::mat2 scaleMat({ scale.x, 0 }, { 0, scale.y });
			glm::mat2 rotate({ glm::cos(rotation), glm::sin(rotation) }, { -glm::sin(rotation), glm::cos(rotation) });
			return rotate * scaleMat * glm::mat2{ 1.f };
		}
	};
}
