#pragma once

#include <optional>
#include <vector>

#include <Logging/Log.h>

#define VK_NO_PROTOTYPES
#include <volk.h>

#define APP_VERSION VK_MAKE_VERSION(1, 0, 0)
#define APP_NAME "Vulkan App"
#define VK_CALL(value) if(value != VK_SUCCESS)  { log_error("VK_CALL failed at %s:%d", __FILE__, __LINE__); }
#define VK_CALL_RET_BOOL(value) if(value != VK_SUCCESS)  { log_error("VK_CALL failed at %s:%d", __FILE__, __LINE__); return false;}

struct VulkanInstance
{
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkSurfaceKHR surface;
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

struct SwapChainSupportDetails
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

	QueueFamilyIndicies queueFamilyIndicies;
	SwapChainSupportDetails swapChainSupportDetails;
};

#ifdef _DEBUG
const bool enableValidationLayers = true;
#elif NDEBUG
const bool enableValidationLayers = false;
#endif
const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

struct GLFWwindow;
bool createVulkanObjects(VulkanInstance& vulkanInstance, VulkanDevice& vulkanDevice, GLFWwindow* window);
void cleanVulkanObjects(VulkanInstance& vulkanInstance, VulkanDevice& vulkanDevice);