#pragma once

#ifdef GRAPHICS_API_OPENGL
#define GL_LOG_FILE "gl.log"
#define FILE_START "OpenGl log file\n"
#elif GRAPHICS_API_VULKAN
#define GL_LOG_FILE "vk.log"
#define FILE_START "Vulkan log file\n"
#endif

namespace kbb
{
	void log_reset();
	void log_info(const char* message, ...);
	void log_error(const char* message, ...);
	void log_error_exception(const char* message, ...);
}