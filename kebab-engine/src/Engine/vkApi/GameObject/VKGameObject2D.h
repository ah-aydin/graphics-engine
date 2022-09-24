#pragma once

#ifdef GRAPHICS_API_VULKAN

#include <Engine/GameObject/GameObject.h>
#include <Engine/GameObject/Transforms/Transform2D.h>

#include <Engine/vkApi/Rendering/Models/VKModel2D.h>

#include <memory>

namespace kbb::vkApi::gameObject
{
	using rendering::models::VKModel2D;

	class VKGameObject2D : public GameObject
	{
		friend class GameObject;

	public:
		using id_t = unsigned int;

		static VKGameObject2D createGameObject()
		{
			VKGameObject2D gameObject = GameObject::createGameObject<VKGameObject2D>();
			return gameObject;
		}

		VKGameObject2D(const VKGameObject2D&) = delete;
		VKGameObject2D& operator=(const VKGameObject2D&) = delete;
		VKGameObject2D(VKGameObject2D&&) = default;
		VKGameObject2D& operator=(VKGameObject2D&&) = default;

		std::shared_ptr<VKModel2D> m_model{};
		glm::vec3 m_color{};
		Transform2D m_transform{};

	private:
		VKGameObject2D(id_t objId) : GameObject(objId) {}
	};
}
#endif