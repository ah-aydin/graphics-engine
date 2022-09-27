#pragma once

#include <Engine/GameObject/GameObject.h>
#include <Engine/GameObject/Transforms/Transform2D.h>

#include <Engine/glApi/Rendering/Models/GLMesh2D.h>

#include <memory>

namespace kbb::glApi
{
	class GLGameObject2D : public GameObject
	{
		friend class GameObject;

	public:
		using id_t = unsigned int;

		static GLGameObject2D createGameObject()
		{
			GLGameObject2D gameObject = GameObject::createGameObject<GLGameObject2D>();
			return gameObject;
		}

		GLGameObject2D(const GLGameObject2D&) = delete;
		GLGameObject2D& operator=(const GLGameObject2D&) = delete;
		GLGameObject2D(GLGameObject2D&&) = default;
		GLGameObject2D& operator=(GLGameObject2D&&) = default;

		std::shared_ptr<GLMesh2D> m_model{};
		Transform2D m_transform{};

	private:
		GLGameObject2D(id_t objId) : GameObject(objId) {}
	};
}
