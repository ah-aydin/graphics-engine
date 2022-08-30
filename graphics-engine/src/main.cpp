#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#ifdef GRAPHICS_API_OPENGL
#include <Engine/glFramework/GLApp.h>
#elif GRAPHICS_API_VULKAN
#include <Engine/vkFramework/VKApp.h>
#endif

int main(int argc, char** argv)
{
#ifdef GRAPHICS_API_OPENGL
	GLApp application;
#elif GRAPHICS_API_VULKAN
	VKApp application;
#endif
	return application.run();
}