#include "Cubes.h"

#ifdef GRAPHICS_API_VULKAN

#include <memory>

namespace kbb::vkApi::primitives::models
{	
	std::shared_ptr<VKMesh3D> createPosV3ColorV3(VulkanDevice& device, glm::vec3 offset)
	{
		std::vector<Vertex3D> vertices
		{
			// left face
			{{-.5f, -.5f, -.5f}},
			{{-.5f, .5f, .5f}},
			{{-.5f, -.5f, .5f}},
			{{-.5f, -.5f, -.5f}},
			{{-.5f, .5f, -.5f}},
			{{-.5f, .5f, .5f}},

			// right face
			{{.5f, -.5f, -.5f}},
			{{.5f, .5f, .5f}},
			{{.5f, -.5f, .5f}},
			{{.5f, -.5f, -.5f}},
			{{.5f, .5f, -.5f}},
			{{.5f, .5f, .5f}},

			// top face
			{{-.5f, -.5f, -.5f}},
			{{.5f, -.5f, .5f}},
			{{-.5f, -.5f, .5f}},
			{{-.5f, -.5f, -.5f}},
			{{.5f, -.5f, -.5f}},
			{{.5f, -.5f, .5f}},

			// bottom face
			{{-.5f, .5f, -.5f}},
			{{.5f, .5f, .5f}},
			{{-.5f, .5f, .5f}},
			{{-.5f, .5f, -.5f}},
			{{.5f, .5f, -.5f}},
			{{.5f, .5f, .5f}},

			// nose face
			{{-.5f, -.5f, 0.5f}},
			{{.5f, .5f, 0.5f}},
			{{-.5f, .5f, 0.5f}},
			{{-.5f, -.5f, 0.5f}},
			{{.5f, -.5f, 0.5f}},
			{{.5f, .5f, 0.5f}},

			// tail face
			{{-.5f, -.5f, -0.5f}},
			{{.5f, .5f, -0.5f}},
			{{-.5f, .5f, -0.5f}},
			{{-.5f, -.5f, -0.5f}},
			{{.5f, -.5f, -0.5f}},
			{{.5f, .5f, -0.5f}},

		};
		for (auto& v : vertices)
		{
			v.position += offset;
		}

		// These will not work properly with culling on
		// did not take into account the clockwise indexing
		std::vector<unsigned int> indices =
		{
			0, 1, 2,
			3, 4, 5,

			6,7,8,
			9,10,11,

			12,13,14,
			15,16,17,

			18,19,20,
			21,22,23,

			24,25,26,
			27,28,29,

			30,31,32,
			33,34,35,
		};

		return std::make_shared<VKMesh3D>(device, vertices, indices, INDEX_DRAW);
	} 
}

#endif