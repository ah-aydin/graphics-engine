#pragma once

#ifdef GRAPHICS_API_OPENGL

#include <glad/glad.h>

namespace glApi::rendering::shaders
{
	GLenum GLShaderTypeFromFileName(const char* fileName);
}
#endif