#include "BaseApp.h"

#include <Logging/Log.h>

#include <GLFW/glfw3.h>

BaseApp::BaseApp()
{
}

BaseApp::~BaseApp()
{
}

bool BaseApp::init()
{
	log_reset();
	if (glfwInit() == GLFW_FALSE)
	{
		log_error("ERROR::GLFW::INITIALIZATION");
		return false;
	}
	initInput();
	return true;
}

void BaseApp::quit()
{
	glfwTerminate();
}