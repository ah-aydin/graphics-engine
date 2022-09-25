#pragma once

#ifdef GRAPHICS_API_VULKAN

#include <Logging/Log.h>

namespace kbb::vkApi
{
#ifdef NDEBUG
	#define VK_CALL(vkFunc) vkFunc
#else
	#define VK_CALL(vkFunc) if (vkFunc != VK_SUCCESS) { log_error_exception("VK_CALL failed at %s:%d", __FILE__, __LINE__); }
#endif
}
#endif
