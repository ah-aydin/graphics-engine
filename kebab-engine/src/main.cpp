#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#ifdef GRAPHICS_API_OPENGL
    #include <Engine/glApi/GLApplication.h>
#elif GRAPHICS_API_VULKAN
    #include <Engine/vkApi/VKApplication.h>
#else
    error(No graphics api was specified)
#endif

#include <Logging/Log.h>

int main(int argc, char** argv)
{
#ifdef GRAPHICS_API_OPENGL
	kbb::glApi::GLApplication app;
#elif GRAPHICS_API_VULKAN
    kbb::vkApi::VKApplication app;
#endif
    try
    {
        app.run();
    }
    catch (const std::exception& e)
    {
        kbb::log_error("%s", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}