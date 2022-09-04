#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <stdexcept>
#include <Logging/Log.h>

#ifdef GRAPHICS_API_OPENGL
#include <Engine/glApi/GLApplication.h>
#endif

int main(int argc, char** argv)
{
#ifdef GRAPHICS_API_OPENGL
	GLApplication app;
#endif
    try
    {
        app.run();
    }
    catch (const std::exception& e)
    {
        log_error("%s", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}