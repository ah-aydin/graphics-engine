#include "Cube.h"

#ifdef GRAPHICS_API_VULKAN

#include <Engine/vkApi/Rendering/Models/Primitives/Cubes.h>

namespace kbb::vkApi::primitives::gameObjects
{
	VKGameObject3D cube3D(VulkanDevice& vulkanDevice)
	{
		std::shared_ptr<VKMesh3D> cubeModel = models::createPosV3ColorV3(vulkanDevice, { 0, 0, 0 });

		auto cube = VKGameObject3D::createGameObject();
		cube.m_model = cubeModel;
		cube.m_transform.translation = { 0, 0, .5f };
		cube.m_transform.scale = { .5f, .5f, .5f };
		
		return cube;
	}
}

#endif