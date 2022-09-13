#pragma once

#ifdef GRAPHICS_API_VULKAN

#include <Engine/Common/GameObject.h>
#include <Engine/vkApi/Rendering/Models/VKModel3D.h>

#include <glm/gtc/matrix_transform.hpp>

#include <memory>

struct Transform3D
{
	glm::vec3 translation{};
	glm::vec3 scale{ 1.0f, 1.0f, 1.0f };
	glm::vec3 rotation{};

	glm::mat4 mat4()
	{
        // Yoinked formula from wikipedia
        const float c3 = glm::cos(rotation.z);
        const float s3 = glm::sin(rotation.z);
        const float c2 = glm::cos(rotation.x);
        const float s2 = glm::sin(rotation.x);
        const float c1 = glm::cos(rotation.y);
        const float s1 = glm::sin(rotation.y);
        return glm::mat4{
            {
                scale.x * (c1 * c3 + s1 * s2 * s3),
                scale.x * (c2 * s3),
                scale.x * (c1 * s2 * s3 - c3 * s1),
                0.0f,
            },
            {
                scale.y * (c3 * s1 * s2 - c1 * s3),
                scale.y * (c2 * c3),
                scale.y * (c1 * c3 * s2 + s1 * s3),
                0.0f,
            },
            {
                scale.z * (c2 * s1),
                scale.z * (-s2),
                scale.z * (c1 * c2),
                0.0f,
            },
            {translation.x, translation.y, translation.z, 1.0f} 
        };
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

	std::shared_ptr<VKModel3D> m_model{};
	glm::vec3 m_color{};
	Transform3D m_transform{};

private:
	VKGameObject3D(id_t objId) : GameObject(objId) {}
};

#endif