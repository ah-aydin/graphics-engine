#include "VKApp.h"

#ifdef GRAPHICS_API_VULKAN

#include <GLFW/glfw3.h>

#include <Engine/Input.h>
#include <Engine/Time.h>
#include <Engine/Settings.h>
#include <Logging/Log.h>

VKApp::~VKApp()
{
	glfwTerminate();
}

int VKApp::run()
{
	if (init() == false)
		return EXIT_FAILURE;

	mainLoop();

	quit();

	return EXIT_SUCCESS;
}

bool VKApp::init()
{
	VK_CALL_RET_BOOL(volkInitialize());
	if (!BaseApp::init() || !window.init())
		return false;

	createVulkanObjects(vulkanInstance, vulkanDevice, window.getGlfwWindowPointer());

	return true;
}

void VKApp::quit()
{
	cleanVulkanObjects(vulkanInstance, vulkanDevice);
	BaseApp::quit();
}

void VKApp::initInput()
{
	Input::createAction("QUIT", GLFW_KEY_ESCAPE);
}

void VKApp::mainLoop()
{
	Settings::ratio = (float)window.getWidth() / window.getHeight();

	while (running)
	{
		if (Input::getAction("QUIT")) {
			running = false;
		}

		Input::resetMouse();
		Time::tick();
		window.tick();
	}

	window.setShouldClose(true);
}

#endif