#pragma once

#include <Engine/GameObject/GameObject.h>
#include <Engine/GameObject/Transforms/Transform3D.h>

#include <Engine/glApi/Rendering/Models/GLMesh3D.h>

#include <memory>

#ifdef GRAPHICS_API_OPENGL

namespace kbb::glApi
{
	class GLGameObject3D : public GameObject
	{
		friend class GameObject;

	public:
		using id_t = unsigned int;

		static GLGameObject3D createGameObject()
		{
			GLGameObject3D gameObject = GameObject::createGameObject<GLGameObject3D>();
			return gameObject;
		}

		GLGameObject3D(const GLGameObject3D&) = delete;
		GLGameObject3D& operator=(const GLGameObject3D&) = delete;
		GLGameObject3D(GLGameObject3D&&) = default;
		GLGameObject3D& operator=(GLGameObject3D&&) = default;

		std::shared_ptr<GLMesh3D> m_model{};
		Transform3D m_transform{};

	private:
		GLGameObject3D(id_t objId) : GameObject(objId) {}
	};
}

#endif
