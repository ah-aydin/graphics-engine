#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <vector>
#include <optional>

#define MAX_FRAMES_IN_FLIGHT 2

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


class VKApp
{
public:
	void run();
	bool frameBufferResized = false;

private:
	void initWindow();
	void initVulkan();
	void mainLoop();
	void cleanup();

	// Rendering
	uint32_t currentFrame = 0;
	void drawFrame();
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	void createSyncObjects();

	// Window
	GLFWwindow* window;

	// Instance
	VkInstance instance;
	void createInstance();


	// Debugging stuff
	const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
	const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif
	bool checkValidationSupport();
	bool checkDeviceExtensionSupport(VkPhysicalDevice  device);
	std::vector<const char*> getRequiredExtensions();

	VkDebugUtilsMessengerEXT debugMessenger;
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	void setupDebugMessenger();

	// Window surface
	VkSurfaceKHR surface;
	void createSurface();

	// GPU
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	int rateDeviceSuitability(VkPhysicalDevice device);
	bool isDeviceSuitable(VkPhysicalDevice device);
	void pickPhysicalDevice();

	QueueFamilyIndicies findQueueFamilies(VkPhysicalDevice device);

	// Logical device
	VkDevice device;
	void createLogicalDevice();

	// Queues
	VkQueue graphicsQueue;
	VkQueue presentQueue;

	// Swap chain
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	SwapchainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtend(const VkSurfaceCapabilitiesKHR& capabilities);
	void createSwapChain();
	void cleanupSwapChain();
	void recreateSwapChain();

	// Image views
	std::vector<VkImageView> swapChainImageViews;
	void createImageViews();

	// Render pass
	VkRenderPass renderPass;
	void createRenderPass();

	// Graphics pipeline
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
	void createGraphicsPipeline();
	VkShaderModule createShaderModule(const std::vector<char>& code);

	// Framebuffers
	std::vector<VkFramebuffer> swapChainFramebuffers;
	void createFramebuffers();

	// Command pools
	VkCommandPool commandPool;
	void createCommandPool();

	// Vertex buffer
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	VkBuffer vertexBuffer, indexBuffer;
	VkDeviceMemory vertexBufferMemory, indexBufferMemory;
	void createVertexBuffer();
	void createIndexBuffer();
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	// Command buffer
	std::vector<VkCommandBuffer> commandBuffers;
	void createCommandBuffers();

	// Command buffer recording
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
};

