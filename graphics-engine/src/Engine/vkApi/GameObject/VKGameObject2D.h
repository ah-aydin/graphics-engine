#pragma once

#ifdef GRAPHICS_API_VULKAN

#include <Engine/vkApi/Rendering/VKModel.h>

#include <memory>

struct Transform2D
{
	glm::vec2 translation{};
	glm::vec2 scale{1.0f, 1.0f};
	float rotation = 0.f;

	glm::mat2 mat2() { 
		glm::mat2 scaleMat({ scale.x, 0 }, { 0, scale.y });
		glm::mat2 rotate({ glm::cos(rotation), glm::sin(rotation) }, { -glm::sin(rotation), glm::cos(rotation) });
		return rotate * scaleMat * glm::mat2{ 1.f };
	}
};

class VKGameObject2D
{
public:
	using id_t = unsigned int;

	static VKGameObject2D createGameObject()
	{
		static id_t currentId = 0;
		return VKGameObject2D{ currentId++ };
	}

	VKGameObject2D(const VKGameObject2D&) = delete;
	VKGameObject2D& operator=(const VKGameObject2D&) = delete;
	VKGameObject2D(VKGameObject2D&&) = default;
	VKGameObject2D& operator=(VKGameObject2D&&) = default;

	id_t getId() { return m_id; }

	std::shared_ptr<VKModel> m_model{};
	glm::vec3 m_color{};
	Transform2D m_transform{};

private:
	VKGameObject2D(id_t objId) : m_id(objId) {}
	id_t m_id;
};

#endif