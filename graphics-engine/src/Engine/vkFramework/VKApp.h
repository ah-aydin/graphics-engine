#pragma once

#include "VKUtils.h"

#include <Engine/BaseApp.h>
#include "VKWindow.h"

class VKApp : public BaseApp
{
public:
	VKApp() : window("Vulkan App") {};
	~VKApp();

private:
	bool init() override;
	void initInput() override;
	void quit() override;

	void mainLoop() override;

	VulkanInstance vulkanInstance;
	VulkanDevice vulkanDevice;

public:
	int run() override;

	VKWindow window;
};