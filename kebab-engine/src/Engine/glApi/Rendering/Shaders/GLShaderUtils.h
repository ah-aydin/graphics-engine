#pragma once

#ifdef GRAPHICS_API_OPENGL

#include <glad/glad.h>

namespace kbb::glApi
{
	GLenum GLShaderTypeFromFileName(const char* fileName);
}
#endif