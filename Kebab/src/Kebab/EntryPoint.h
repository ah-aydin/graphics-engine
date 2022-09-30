#pragma once

#include <kbbpch.h>

#include "Kebab.h"

extern kbb::Application* kbb::CreateApplication();

int main(int argc, char** argv)
{
	kbb::Log::Init();

	auto app = kbb::CreateApplication();
	try
	{
		app->run();
	} 
    catch (const std::exception& e)
    {
        KBB_ERROR("{0}", e.what());
		delete app;
        return EXIT_FAILURE;
    }
	delete app;

	return 0;
}
