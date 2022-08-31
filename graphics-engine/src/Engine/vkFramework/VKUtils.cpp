#include "VKUtils.h"

#define VK_NO_PROTOTYPES
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <assert.h>
#include <string.h>
#include <string>
#include <map>
#include <set>

#include <Logging/Log.h>

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT          messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT                 messageTypes,
	const VkDebugUtilsMessengerCallbackDataEXT*		pCallbackData,
	void*											pUserData)
{
	if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
	{
		log_error("-------------------------------ERROR-------------------------------");
	}
	log_error("VULKAN::VALIDATION_LAYER::%s", pCallbackData->pMessage);

	return VK_FALSE;
}

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& info)
{
	info = {};
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	info.messageSeverity = 
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	info.messageType = 
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	info.pfnUserCallback = debugCallback;
	info.pUserData = nullptr; // Optional
}

QueueFamilyIndicies findQueueFamilies(VkPhysicalDevice gpu, const VkSurfaceKHR& surface, const VkInstance& instance)
{
	QueueFamilyIndicies indicies;

	uint32_t count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(gpu, &count, nullptr);

	std::vector<VkQueueFamilyProperties> qfps(count);
	vkGetPhysicalDeviceQueueFamilyProperties(gpu, &count, qfps.data());

	int i = 0;
	for (const VkQueueFamilyProperties& family : qfps)
	{
		if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indicies.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(gpu, i, surface, &presentSupport);
		if (presentSupport)
		{
			indicies.presentFamily = i;
		}

		if (indicies.isComplete()) break;

		i++;
	}

	return indicies;
}

SwapChainSupportDetails querySwapChainSupportDetails(VkPhysicalDevice gpu, const VkSurfaceKHR& surface, const VkInstance& instance)
{
	SwapChainSupportDetails details;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, surface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &formatCount, nullptr);
	if (formatCount != 0)
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &presentModeCount, nullptr);
	if (presentModeCount != 0)
	{
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

bool createVulkanObjects(VulkanInstance& vulkanInstance, VulkanDevice& vulkanDevice, GLFWwindow* window)
{
	// Enable validation layers if on debug and available
	if (enableValidationLayers)
	{
		uint32_t count;
		VK_CALL(vkEnumerateInstanceLayerProperties(&count, nullptr));

		std::vector<VkLayerProperties> layers(count);
		VK_CALL(vkEnumerateInstanceLayerProperties(&count, layers.data()));

		for (const char* layerName : validationLayers)
		{
			bool layerFound = false;

			for (const VkLayerProperties& layer : layers)
			{
				if (strcmp(layer.layerName, layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound)
			{
				return false;
			}
		}
	}

	// Log out vulkan extensions
	{
		uint32_t count = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);

		std::vector<VkExtensionProperties> extensions(count);
		vkEnumerateInstanceExtensionProperties(nullptr, &count, extensions.data());
		
		log_info("Available VULKAN extensions");
		for (unsigned int i = 0; i < count; ++i)
		{
			log_info("\t%s", extensions[i].extensionName);
		}
	}

	// VkInstance
	{
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = APP_NAME;
		appInfo.applicationVersion = APP_VERSION;
		appInfo.pEngineName = APP_NAME;
		appInfo.engineVersion = APP_VERSION;
		appInfo.apiVersion = VK_MAKE_VERSION(1, 0, 0);

		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		log_info("Available GLFW extensions");
		for (unsigned int i = 0; i < glfwExtensionCount; ++i)
		{
			log_info("\t%s", glfwExtensions[i]);
		}
		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (enableValidationLayers)
		{
			extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
		}

		VkInstanceCreateInfo instanceInfo{};
		instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceInfo.pApplicationInfo = &appInfo;
		instanceInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		instanceInfo.ppEnabledExtensionNames = extensions.data();
		if (enableValidationLayers)
		{
			instanceInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			instanceInfo.ppEnabledLayerNames = validationLayers.data();

			VkDebugUtilsMessengerCreateInfoEXT debugInfo{};
			populateDebugMessengerCreateInfo(debugInfo);
			instanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)& debugInfo;
		}
		else
		{
			instanceInfo.enabledLayerCount = 0;
			instanceInfo.pNext = nullptr;
		}

		VK_CALL_RET_BOOL(vkCreateInstance(&instanceInfo, nullptr, &vulkanInstance.instance));

		volkLoadInstance(vulkanInstance.instance);
	}

	// Debug messenger
	if (enableValidationLayers)
	{
		VkDebugUtilsMessengerCreateInfoEXT info{};
		populateDebugMessengerCreateInfo(info);
		VK_CALL_RET_BOOL(vkCreateDebugUtilsMessengerEXT(vulkanInstance.instance, &info, nullptr, &vulkanInstance.debugMessenger));
	}

	// VkSurfaceKHR
	{
		VK_CALL_RET_BOOL(glfwCreateWindowSurface(vulkanInstance.instance, window, nullptr, &vulkanInstance.surface));
	}

	// Choose VkPhysicalDevice (GPU)
	{
		uint32_t gpuCount = 0;
		vkEnumeratePhysicalDevices(vulkanInstance.instance, &gpuCount, nullptr);

		std::vector<VkPhysicalDevice> gpus(gpuCount);
		vkEnumeratePhysicalDevices(vulkanInstance.instance, &gpuCount, gpus.data());

		std::multimap<int, VkPhysicalDevice> candidates;

		for (const VkPhysicalDevice& gpu : gpus)
		{
			// Check queue family indicies
			{
				QueueFamilyIndicies indicies = findQueueFamilies(gpu, vulkanInstance.surface, vulkanInstance.instance);
				if (!indicies.isComplete()) continue;
			}

			// Check device suitability
			{
				// Check extensions
				uint32_t extensionCount;
				vkEnumerateDeviceExtensionProperties(gpu, nullptr, &extensionCount, nullptr);

				std::vector<VkExtensionProperties> availableExtensions(extensionCount);
				vkEnumerateDeviceExtensionProperties(gpu, nullptr, &extensionCount, availableExtensions.data());

				std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
				for (const VkExtensionProperties& extension : availableExtensions)
				{
					requiredExtensions.erase(extension.extensionName);
				}

				if (!requiredExtensions.empty()) continue;

				// Check swapchain support
				SwapChainSupportDetails details = querySwapChainSupportDetails(gpu, vulkanInstance.surface, vulkanInstance.instance);
				if (details.formats.empty() || details.presentModes.empty()) continue;
			}
			
			// Calculate device score
			{
				VkPhysicalDeviceProperties properties;
				vkGetPhysicalDeviceProperties(gpu, &properties);

				VkPhysicalDeviceFeatures features;
				vkGetPhysicalDeviceFeatures(gpu, &features);

				int score = 0;
				if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) score += 1000;
				score += properties.limits.maxImageDimension2D;
				if (!features.geometryShader) score = 0;

				if (score == 0) continue;
				candidates.insert(std::make_pair(score, gpu));
			}
		}

		if (candidates.size() == 0)
		{
			log_error("ERROR::VULKAN::INITIALIZATION::Failed to find GPU with requested properties and features");
			return false;
		}
		// Get the highest scoring gpu
		int maxScore = 0;
		for (auto candidate = candidates.begin(); candidate != candidates.end(); ++candidate)
		{
			if (maxScore < candidate->first)
			{
				maxScore = candidate->first;
			}
		}
		vulkanDevice.physicalDevice = candidates.find(maxScore)->second;
	}

	// VkDevice
	{
		QueueFamilyIndicies indices = findQueueFamilies(vulkanDevice.physicalDevice, vulkanInstance.surface, vulkanInstance.instance);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };
	
		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies)
		{
			VkDeviceQueueCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			createInfo.queueCount = 1;
			createInfo.pQueuePriorities = &queuePriority;
			createInfo.queueFamilyIndex = queueFamily;
			queueCreateInfos.push_back(createInfo);
		}

		VkPhysicalDeviceFeatures features{};

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.pEnabledFeatures = &features;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();
		if (enableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else
		{
			createInfo.enabledLayerCount = 0;
		}

		VK_CALL_RET_BOOL(vkCreateDevice(vulkanDevice.physicalDevice, &createInfo, nullptr, &vulkanDevice.device));
		vkGetDeviceQueue(vulkanDevice.device, indices.graphicsFamily.value(), 0, &vulkanDevice.graphicsQueue);
		vkGetDeviceQueue(vulkanDevice.device, indices.presentFamily.value(), 0, &vulkanDevice.presentQueue);

		volkLoadDevice(vulkanDevice.device);
	}

	return true;
}

void cleanVulkanObjects(VulkanInstance& vulkanInstance, VulkanDevice& vulkanDevice)
{
	vkDestroyDevice(vulkanDevice.device, nullptr);

	if (enableValidationLayers)
	{
		vkDestroyDebugUtilsMessengerEXT(vulkanInstance.instance, vulkanInstance.debugMessenger, nullptr);
	}

	vkDestroySurfaceKHR(vulkanInstance.instance, vulkanInstance.surface, nullptr);
	vkDestroyInstance(vulkanInstance.instance, nullptr);
}
