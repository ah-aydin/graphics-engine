#include "VKObjects.h"

#define VK_NO_PROTOTYPES
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <assert.h>
#include <string.h>
#include <string>
#include <map>
#include <set>
#include <algorithm>

#include <Common/Utils/fileUtils.h>
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

SwapchainSupportDetails querySwapchainSupportDetails(VkPhysicalDevice gpu, const VkSurfaceKHR& surface, const VkInstance& instance)
{
	SwapchainSupportDetails details;
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

bool createSwapchain(VulkanInstance& vulkanInstance, VulkanDevice& vulkanDevice, GLFWwindow* window)
{
	SwapchainSupportDetails swapchainDetails = querySwapchainSupportDetails(vulkanDevice.physicalDevice, vulkanInstance.surface, vulkanInstance.instance);

	// Choose surface format
	VkSurfaceFormatKHR surfaceFormat;
	{
		bool found = false;
		for (const VkSurfaceFormatKHR& format : swapchainDetails.formats)
		{
			if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				found = true;
				surfaceFormat = format;
				break;
			}
		}
		if (!found) surfaceFormat = swapchainDetails.formats[0];
	}

	// Choose present mode
	VkPresentModeKHR presentMode;
	{
		bool found = false;
		for (const VkPresentModeKHR& mode : swapchainDetails.presentModes)
		{
			if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				found = true;
				presentMode = mode;
				break;
			}
		}
		if (!found) presentMode = VK_PRESENT_MODE_FIFO_KHR;
	}

	// Choose swap extent
	VkExtent2D extent;
	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		VkExtent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		actualExtent.width = std::clamp(actualExtent.width, swapchainDetails.capabilities.minImageExtent.width, swapchainDetails.capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, swapchainDetails.capabilities.minImageExtent.height, swapchainDetails.capabilities.maxImageExtent.height);

		extent = actualExtent;
	}

	uint32_t imageCount = swapchainDetails.capabilities.minImageCount + 1;
	if (imageCount > swapchainDetails.capabilities.maxImageCount)
	{
		imageCount = swapchainDetails.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = vulkanInstance.surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndicies indices = findQueueFamilies(vulkanDevice.physicalDevice, vulkanInstance.surface, vulkanInstance.instance);
	uint32_t queueFamilyIndicies[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };
	if (indices.graphicsFamily != indices.presentFamily)
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndicies;
	}
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;
	}

	createInfo.preTransform = swapchainDetails.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	VK_CALL_RET_BOOL(vkCreateSwapchainKHR(vulkanDevice.device, &createInfo, nullptr, &vulkanDevice.swapchain));

	vkGetSwapchainImagesKHR(vulkanDevice.device, vulkanDevice.swapchain, &imageCount, nullptr);
	vulkanDevice.swapchainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(vulkanDevice.device, vulkanDevice.swapchain, &imageCount, vulkanDevice.swapchainImages.data());

	vulkanDevice.swapchainImageFormat = surfaceFormat.format;
	vulkanDevice.swapchainExtent = extent;

	return true;
}

bool createImageViews(VulkanDevice& vulkanDevice)
{
	vulkanDevice.swapchainImageViews.resize(vulkanDevice.swapchainImages.size());
	for (size_t i = 0; i < vulkanDevice.swapchainImages.size(); ++i)
	{
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = vulkanDevice.swapchainImages[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = vulkanDevice.swapchainImageFormat;

		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
	
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		VK_CALL_RET_BOOL(vkCreateImageView(vulkanDevice.device, &createInfo, nullptr, &vulkanDevice.swapchainImageViews[i]));
	}

	return true;
}

VkShaderModule createShaderModule(VkDevice device, std::vector<char> code)
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule module;
	VK_CALL(vkCreateShaderModule(device, &createInfo, nullptr, &module));

	return module;
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
			instanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugInfo;
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
				SwapchainSupportDetails details = querySwapchainSupportDetails(gpu, vulkanInstance.surface, vulkanInstance.instance);
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
		vulkanDevice.queueFamilyIndicies = findQueueFamilies(vulkanDevice.physicalDevice, vulkanInstance.surface, vulkanInstance.instance);
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

	// VkSwapchainKHR
	if (!createSwapchain(vulkanInstance, vulkanDevice, window)) return false;

	// Image views
	if (!createImageViews(vulkanDevice)) return false;

	// Render passes
	{
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = vulkanDevice.swapchainImageFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = 0;

		VkRenderPassCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		createInfo.attachmentCount = 1;
		createInfo.pAttachments = &colorAttachment;
		createInfo.subpassCount = 1;
		createInfo.pSubpasses = &subpass;
		createInfo.dependencyCount = 1;
		createInfo.pDependencies = &dependency;

		VK_CALL_RET_BOOL(vkCreateRenderPass(vulkanDevice.device, &createInfo, nullptr, &vulkanDevice.renderPass));
	}

	// Graphics pipeline
	{
		// Pipeline stage create infos
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
		VkShaderModule vertShaderModule, fragShaderModule;
		{
			auto vertShaderCode = readSPIRV("res/vulkanvert.spv");
			auto fragShaderCode = readSPIRV("res/vulkanfrag.spv");

			vertShaderModule = createShaderModule(vulkanDevice.device, vertShaderCode);
			fragShaderModule = createShaderModule(vulkanDevice.device, fragShaderCode);

			VkPipelineShaderStageCreateInfo vertInfo{};
			vertInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			vertInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
			vertInfo.module = vertShaderModule;
			vertInfo.pName = "main";
			shaderStages.push_back(vertInfo);

			VkPipelineShaderStageCreateInfo fragInfo{};
			fragInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			fragInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			fragInfo.module = fragShaderModule;
			fragInfo.pName = "main";
			shaderStages.push_back(fragInfo);
		}

		// Replace the default viewport and scissor to specify them during runtime
		VkPipelineDynamicStateCreateInfo dynamicState{};
		std::vector<VkDynamicState> states = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		{
			dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			dynamicState.dynamicStateCount = static_cast<uint32_t>(states.size());
			dynamicState.pDynamicStates = states.data();

		}

		VkPipelineVertexInputStateCreateInfo vertexInput{};
		{
			vertexInput.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			auto description = VulkanVertex::getBindingDescription();
			vertexInput.vertexBindingDescriptionCount = 1;
			vertexInput.pVertexBindingDescriptions = &description;
			vertexInput.vertexAttributeDescriptionCount = static_cast<uint32_t>(VulkanVertex::getDescriptions().size());
			vertexInput.pVertexAttributeDescriptions = VulkanVertex::getDescriptions().data();
		}

		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		{
			inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			inputAssembly.primitiveRestartEnable = VK_FALSE;
		}

		VkPipelineViewportStateCreateInfo viewportState{};
		{
			VkViewport viewport{};
			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = (float)vulkanDevice.swapchainExtent.width;
			viewport.height = (float)vulkanDevice.swapchainExtent.height;
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;

			VkRect2D scissor{};
			scissor.offset = { 0, 0 };
			scissor.extent = vulkanDevice.swapchainExtent;

			viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			viewportState.viewportCount = 1;
			viewportState.pViewports = &viewport;
			viewportState.scissorCount = 1;
			viewportState.pScissors = &scissor;
		}

		VkPipelineRasterizationStateCreateInfo rasterizer{};
		{
			rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			rasterizer.depthClampEnable = VK_FALSE;
			rasterizer.rasterizerDiscardEnable = VK_FALSE;
			rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
			rasterizer.lineWidth = 1.0f;
			rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
			rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
			rasterizer.depthBiasEnable = VK_FALSE;
			rasterizer.depthBiasConstantFactor = 0.0f;
			rasterizer.depthBiasClamp = 0.0f;
			rasterizer.depthBiasSlopeFactor = 0.0f;
		}

		VkPipelineMultisampleStateCreateInfo multisampling{};
		{
			multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			multisampling.sampleShadingEnable = VK_FALSE;
			multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
			multisampling.minSampleShading = 1.0f;
			multisampling.pSampleMask = nullptr;
			multisampling.alphaToCoverageEnable = VK_FALSE;
			multisampling.alphaToOneEnable = VK_FALSE;
		}

		VkPipelineColorBlendStateCreateInfo colorBlending{};
		{
			VkPipelineColorBlendAttachmentState attachment{};
			attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_A_BIT;
			attachment.blendEnable = VK_TRUE;
			attachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
			attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			attachment.colorBlendOp = VK_BLEND_OP_ADD;
			attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
			attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
			attachment.alphaBlendOp = VK_BLEND_OP_ADD;

			colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			colorBlending.logicOpEnable = VK_FALSE;
			colorBlending.logicOp = VK_LOGIC_OP_COPY;
			colorBlending.attachmentCount = 1;
			colorBlending.pAttachments = &attachment;
			colorBlending.blendConstants[0] = 0.0f;
			colorBlending.blendConstants[1] = 0.0f;
			colorBlending.blendConstants[2] = 0.0f;
			colorBlending.blendConstants[3] = 0.0f;
		}

		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
		pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreateInfo.setLayoutCount = 0;
		pipelineLayoutCreateInfo.pSetLayouts = nullptr;
		pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
		pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

		VK_CALL_RET_BOOL(vkCreatePipelineLayout(vulkanDevice.device, &pipelineLayoutCreateInfo, nullptr, &vulkanDevice.pipelineLayout));

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages.data();
		pipelineInfo.pVertexInputState = &vertexInput;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = nullptr;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicState;
		pipelineInfo.layout = vulkanDevice.pipelineLayout;
		pipelineInfo.renderPass = vulkanDevice.renderPass;
		pipelineInfo.subpass = 0;

		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
		pipelineInfo.basePipelineIndex = -1; // Optional

		VK_CALL_RET_BOOL(vkCreateGraphicsPipelines(vulkanDevice.device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &vulkanDevice.graphicsPipeline));

		vkDestroyShaderModule(vulkanDevice.device, vertShaderModule, nullptr);
		vkDestroyShaderModule(vulkanDevice.device, fragShaderModule, nullptr);
	}

	// Swapchain framebuffers
	{
		vulkanDevice.swapchainFramebuffers.resize(vulkanDevice.swapchainImageViews.size());
		for (size_t i = 0; i < vulkanDevice.swapchainImageViews.size(); ++i)
		{
			std::vector<VkImageView> attachments = { vulkanDevice.swapchainImageViews[i] };

			VkFramebufferCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			createInfo.attachmentCount = attachments.size();
			createInfo.pAttachments = attachments.data();
			createInfo.layers = 1;
			createInfo.renderPass = vulkanDevice.renderPass;
			createInfo.width = vulkanDevice.swapchainExtent.width;
			createInfo.height = vulkanDevice.swapchainExtent.height;

			VK_CALL_RET_BOOL(vkCreateFramebuffer(vulkanDevice.device, &createInfo, nullptr, &vulkanDevice.swapchainFramebuffers[i]));
		}
	}

	// Command pool
	{
		VkCommandPoolCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfo.queueFamilyIndex = vulkanDevice.queueFamilyIndicies.graphicsFamily.value();
		createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		VK_CALL_RET_BOOL(vkCreateCommandPool(vulkanDevice.device, &createInfo, nullptr, &vulkanDevice.commandPool));
	}

	// Command buffers
	{
		vulkanDevice.commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
		
		VkCommandBufferAllocateInfo allocateInfo{};
		allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocateInfo.commandPool = vulkanDevice.commandPool;
		allocateInfo.commandBufferCount = (uint32_t) MAX_FRAMES_IN_FLIGHT;
		allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		VK_CALL_RET_BOOL(vkAllocateCommandBuffers(vulkanDevice.device, &allocateInfo, vulkanDevice.commandBuffers.data()));
	}

	// Sync objects
	{
		vulkanDevice.imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		vulkanDevice.renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		vulkanDevice.inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		{
			VK_CALL_RET_BOOL(vkCreateSemaphore(vulkanDevice.device, &semaphoreInfo, nullptr, &vulkanDevice.imageAvailableSemaphores[i]));
			VK_CALL_RET_BOOL(vkCreateSemaphore(vulkanDevice.device, &semaphoreInfo, nullptr, &vulkanDevice.renderFinishedSemaphores[i]));
			VK_CALL_RET_BOOL(vkCreateFence(vulkanDevice.device, &fenceInfo, nullptr, &vulkanDevice.inFlightFences[i]));
		}
	}

	return true;
}

void cleanSwapchain(VulkanDevice& vulkanDevice)
{
	for (VkFramebuffer framebuffer : vulkanDevice.swapchainFramebuffers)
	{
		vkDestroyFramebuffer(vulkanDevice.device, framebuffer, nullptr);
	}
	for (VkImageView imageView : vulkanDevice.swapchainImageViews)
	{
		vkDestroyImageView(vulkanDevice.device, imageView, nullptr);
	}
	vkDestroySwapchainKHR(vulkanDevice.device, vulkanDevice.swapchain, nullptr);
}

void cleanVulkanObjects(VulkanInstance& vulkanInstance, VulkanDevice& vulkanDevice)
{
	cleanSwapchain(vulkanDevice);

	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
	{
		vkDestroySemaphore(vulkanDevice.device, vulkanDevice.imageAvailableSemaphores[i], nullptr);
		vkDestroySemaphore(vulkanDevice.device, vulkanDevice.renderFinishedSemaphores[i], nullptr);
		vkDestroyFence(vulkanDevice.device, vulkanDevice.inFlightFences[i], nullptr);
	}
	
	vkDestroyCommandPool(vulkanDevice.device, vulkanDevice.commandPool, nullptr);

	vkDestroyPipeline(vulkanDevice.device, vulkanDevice.graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(vulkanDevice.device, vulkanDevice.pipelineLayout, nullptr);
	vkDestroyRenderPass(vulkanDevice.device, vulkanDevice.renderPass, nullptr);

	vkDestroyDevice(vulkanDevice.device, nullptr);

	if (enableValidationLayers)
	{
		vkDestroyDebugUtilsMessengerEXT(vulkanInstance.instance, vulkanInstance.debugMessenger, nullptr);
	}

	vkDestroySurfaceKHR(vulkanInstance.instance, vulkanInstance.surface, nullptr);
	vkDestroyInstance(vulkanInstance.instance, nullptr);
}


void recreateSwapchain(VulkanInstance& vulkanInstance, VulkanDevice& vulkanDevice, GLFWwindow* window)
{
	int width = 0, height = 0;
	glfwGetFramebufferSize(window, &width, &height);
	while (width == 0 || height == 0)
	{
		glfwGetFramebufferSize(window, &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(vulkanDevice.device);

	cleanSwapchain(vulkanDevice);

	createSwapchain(vulkanInstance, vulkanDevice, window);
	createImageViews(vulkanDevice);
}
