#pragma once

#ifdef GRAPHICS_API_VULKAN

#include <Engine/Common/GameObject.h>
#include <Engine/vkApi/Rendering/VKModel2D.h>

#include <glm/gtc/matrix_transform.hpp>

#include <memory>

struct Transform3D
{
	glm::vec3 translation{};
	glm::vec3 scale{ 1.0f, 1.0f, 1.0f };
	glm::vec3 rotation{};

	glm::mat4 mat4()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.f), translation);
		
		transform = glm::rotate(transform, rotation.y, { 0, 1, 0 });
		transform = glm::rotate(transform, rotation.x, { 1, 0, 0 });
		transform = glm::rotate(transform, rotation.z, { 0, 0, 1 });
		
		transform = glm::scale(transform, scale);	
		return transform;
	}
};

class VKGameObject3D : public GameObject
{
	friend class GameObject;

public:
	using id_t = unsigned int;

	static VKGameObject3D createGameObject()
	{
		VKGameObject3D gameObject = GameObject::createGameObject<VKGameObject3D>();
		return gameObject;
	}

	VKGameObject3D(const VKGameObject3D&) = delete;
	VKGameObject3D& operator=(const VKGameObject3D&) = delete;
	VKGameObject3D(VKGameObject3D&&) = default;
	VKGameObject3D& operator=(VKGameObject3D&&) = default;

	std::shared_ptr<VKModel2D> m_model{};
	glm::vec3 m_color{};
	Transform3D m_transform{};

private:
	VKGameObject3D(id_t objId) : GameObject(objId) {}
};

#endif