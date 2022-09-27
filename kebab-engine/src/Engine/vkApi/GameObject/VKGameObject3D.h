#pragma once

#ifdef GRAPHICS_API_VULKAN

#include <Engine/GameObject/GameObject.h>
#include <Engine/GameObject/Transforms/Transform3D.h>

#include <Engine/vkApi/Rendering/Models/VKMesh3D.h>

#include <glm/gtc/matrix_transform.hpp>

#include <memory>

namespace kbb::vkApi
{
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

        std::shared_ptr<VKMesh3D> m_model{};
        glm::vec3 m_color{};
        Transform3D m_transform{};

    private:
        VKGameObject3D(id_t objId) : GameObject(objId) {}
    };
}
#endif