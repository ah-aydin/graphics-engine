#pragma once

#include <assert.h>
#include <optional>
#include <vector>
#include <array>

#include <Logging/Log.h>

#define VK_NO_PROTOTYPES
#include <volk.h>

#include <glm/glm.hpp>

#define APP_VERSION VK_MAKE_VERSION(1, 0, 0)
#define APP_NAME "Vulkan App"
#define VK_CALL(value) if(value != VK_SUCCESS)  { log_error("VK_CALL failed at %s:%d", __FILE__, __LINE__); assert(false); }
#define VK_CALL_RET_BOOL(value) if(value != VK_SUCCESS)  { log_error("VK_CALL failed at %s:%d", __FILE__, __LINE__); return false;}

#define MAX_FRAMES_IN_FLIGHT 2

struct VulkanVertex
{
	glm::vec3 pos;
	glm::vec3 color;

	static VkVertexInputBindingDescription getBindingDescription()
	{
		VkVertexInputBindingDescription description{};
		description.binding = 0;
		description.stride = sizeof(VulkanVertex);
		description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return description;
	}

	static std::array<VkVertexInputAttributeDescription, 2> getDescriptions()
	{
		std::array<VkVertexInputAttributeDescription, 2> descriptions;

		descriptions[0].binding = 0;
		descriptions[0].location = 0;
		descriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		descriptions[0].offset = offsetof(VulkanVertex, pos);

		descriptions[1].binding = 0;
		descriptions[1].location = 1;
		descriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		descriptions[1].offset = offsetof(VulkanVertex, color);

		return descriptions;
	}
};

struct VulkanInstance
{
	VkInstance instance;
	VkSurfaceKHR surface;
	VkDebugUtilsMessengerEXT debugMessenger;
};

struct QueueFamilyIndicies
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete()
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapchainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct VulkanDevice
{
	VkPhysicalDevice physicalDevice;
	VkDevice device;

	VkQueue graphicsQueue;
	VkQueue presentQueue;

	VkSwapchainKHR swapchain;
	std::vector<VkImage> swapchainImages;
	VkFormat swapchainImageFormat;
	VkExtent2D swapchainExtent;
	std::vector<VkImageView> swapchainImageViews;
	std::vector<VkFramebuffer> swapchainFramebuffers;

	VkRenderPass renderPass;

	VkPipeline graphicsPipeline;
	VkPipelineLayout pipelineLayout;

	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;

	QueueFamilyIndicies queueFamilyIndicies;
	SwapchainSupportDetails swapChainSupportDetails;
};

#ifdef _DEBUG
const bool enableValidationLayers = true;
#elif NDEBUG
const bool enableValidationLayers = false;
#endif
const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

// Initializing vulkan
struct GLFWwindow;
bool createVulkanObjects(VulkanInstance& vulkanInstance, VulkanDevice& vulkanDevice, GLFWwindow* window);
void cleanVulkanObjects(VulkanInstance& vulkanInstance, VulkanDevice& vulkanDevice);
void recreateSwapchain(VulkanInstance& vulkanInstance, VulkanDevice& vulkanDeivce, GLFWwindow* window);