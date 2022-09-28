#include "Triangles.h"

#include <Engine/glApi/Rendering/Models/Primitives/Triangles.h>

namespace kbb::glApi::primitives::gameObject
{
	GLGameObject2D triangle()
	{
		GLGameObject2D gameObject = GLGameObject2D::createGameObject();
		gameObject.m_transform.scale = { 1.25f, 1.25f };
		gameObject.m_transform.translation = { 0.0f, 0.0f };
		gameObject.m_transform.rotation = glm::radians(0.0f);
		gameObject.m_model = primitives::mesh::triangle2D();

		return gameObject;
	}
}