#pragma once

#include <Kebab/Log.h>

#include <glad/glad.h>

#ifdef NDEBUG
#define KBB_GL_CALL(glFunc) glFunc;
#else
#define KBB_GL_CALL(glFunc) glFunc; if (glGetError() != GL_NO_ERROR) { KBB_CORE_ERROR("GL_CALL failed.")}
#endif
