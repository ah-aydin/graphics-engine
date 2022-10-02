#pragma once

#include <Kebab/Log.h>

#define VK_NO_PROTOTYPES
#include <volk.h>

#ifdef NDEBUG
	#define KBB_VK_CALL(vkFunc) vkFunc
#else
	#define KBB_VK_CALL(vkFunc) if (vkFunc != VK_SUCCESS) { KBB_CORE_ASSERTION(false, "KBB_VK_CALL failed."); }
#endif

