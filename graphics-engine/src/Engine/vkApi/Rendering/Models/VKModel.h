#pragma once

#ifdef GRAPHICS_API_VULKAN

#include <Engine/vkApi/VulkanDevice.h>

#define VK_NO_PROTOTYPES
#include <volk.h>

#include <cassert>
#include <vector>

namespace vkApi::rendering::models
{
	enum DrawMode {
		VERTEX_DRAW, INDEX_DRAW
	};

	template <typename VertexType>
	class VKModel
	{
	protected:
		VKModel(
			VulkanDevice& vulkanDevice,
			const std::vector<VertexType>& verticies,
			const std::vector<unsigned int>& indices = std::vector<unsigned int>{},
			DrawMode drawMode = VERTEX_DRAW
		)
			: m_vulkanDevice(vulkanDevice), m_drawMode(drawMode)
		{
			createVertexBuffers(verticies);
			if (drawMode == INDEX_DRAW)
			{
				createIndexBuffers(indices);
			}
		}

		virtual ~VKModel()
		{
			vkDestroyBuffer(m_vulkanDevice.device(), m_vertexBuffer, nullptr);
			vkFreeMemory(m_vulkanDevice.device(), m_vertexBufferMemory, nullptr);

			if (m_drawMode == INDEX_DRAW)
			{
				vkDestroyBuffer(m_vulkanDevice.device(), m_indexBuffer, nullptr);
				vkFreeMemory(m_vulkanDevice.device(), m_indexBufferMemory, nullptr);
			}
		};

		VKModel(const VKModel&) = delete;
		VKModel& operator=(const VKModel&) = delete;

	protected:
		void bind(VkCommandBuffer commandBuffer)
		{
			VkBuffer buffers[] = { m_vertexBuffer };
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

			if (m_drawMode == INDEX_DRAW)
			{
				vkCmdBindIndexBuffer(commandBuffer, m_indexBuffer, 0, VK_INDEX_TYPE_UINT32);
			}
		}

		void draw(VkCommandBuffer commandBuffer)
		{
			if (m_drawMode == VERTEX_DRAW)
			{
				vkCmdDraw(commandBuffer, m_vertexCount, 1, 0, 0);
			}
			else if (m_drawMode == INDEX_DRAW)
			{
				vkCmdDrawIndexed(commandBuffer, m_indexCount, 1, 0, 0, 0);
			}
		}

	protected:
		void createVertexBuffers(const std::vector<VertexType>& verticies)
		{
			m_vertexCount = static_cast<uint32_t>(verticies.size());
			assert(m_vertexCount >= 3 && "Vertex count must be at least 3");
			VkDeviceSize bufferSize = sizeof(verticies[0]) * m_vertexCount;

			m_vulkanDevice.createBuffer(
				bufferSize,
				VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
				m_vertexBuffer,
				m_vertexBufferMemory
			);

			void* data;
			vkMapMemory(m_vulkanDevice.device(), m_vertexBufferMemory, 0, bufferSize, 0, &data);
			memcpy(data, verticies.data(), static_cast<size_t>(bufferSize));
			vkUnmapMemory(m_vulkanDevice.device(), m_vertexBufferMemory);
		}

		void createIndexBuffers(const std::vector<unsigned int>& indices)
		{
			m_indexCount = static_cast<uint32_t>(indices.size());
			assert(m_indexCount >= 3 && "Index count must be at least 3");
			VkDeviceSize bufferSize = sizeof(indices[0]) * m_indexCount;

			m_vulkanDevice.createBuffer(
				bufferSize,
				VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
				m_indexBuffer,
				m_indexBufferMemory
			);

			void* data;
			vkMapMemory(m_vulkanDevice.device(), m_indexBufferMemory, 0, bufferSize, 0, &data);
			memcpy(data, indices.data(), static_cast<size_t>(bufferSize));
			vkUnmapMemory(m_vulkanDevice.device(), m_indexBufferMemory);
		}

		VulkanDevice& m_vulkanDevice;
		VkBuffer m_vertexBuffer;
		VkBuffer m_indexBuffer;
		VkDeviceMemory m_vertexBufferMemory;
		VkDeviceMemory m_indexBufferMemory;
		uint32_t m_vertexCount;
		uint32_t m_indexCount;

		DrawMode m_drawMode;
	};
}
#endif