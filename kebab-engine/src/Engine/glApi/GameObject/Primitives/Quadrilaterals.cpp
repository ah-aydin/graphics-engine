#include "Quadrilaterlas.h"

#include <Engine/glApi/Rendering/Models/Primitives/Quadrilaterlas.h>

namespace kbb::glApi::primitives::gameObject
{
	GLGameObject2D square()
	{
		GLGameObject2D gameObject = GLGameObject2D::createGameObject();
		gameObject.m_transform.translation = { 0.0f, 0.0f };
		gameObject.m_transform.scale = { 1.0f, 1.0f };
		gameObject.m_transform.rotation = glm::radians(0.0f);
		gameObject.m_model = primitives::mesh::square();

		return gameObject;
	}
}