#pragma once

#include <glad/glad.h>
#include <cstdint>

namespace kbb::glApi
{
	struct VertexAttribPointer
	{
		GLuint type;
		GLint count;
		uint64_t offset;
	};
}
