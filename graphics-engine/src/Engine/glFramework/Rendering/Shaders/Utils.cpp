#include "Utils.h"

#include <assert.h>

#include <Common/Utils/fileUtils.h>

GLenum GLShaderTypeFromFileName(const char* fileName)
{
	if (endsWith(fileName, ".vert")) return GL_VERTEX_SHADER;
	if (endsWith(fileName, ".frag")) return GL_FRAGMENT_SHADER;
	if (endsWith(fileName, ".geom")) return GL_GEOMETRY_SHADER;
	if (endsWith(fileName, ".tesc")) return GL_TESS_CONTROL_SHADER;
	if (endsWith(fileName, ".tese")) return GL_TESS_EVALUATION_SHADER;
	if (endsWith(fileName, ".comp")) return GL_COMPUTE_SHADER;

	assert(false);

	return 0;
}
