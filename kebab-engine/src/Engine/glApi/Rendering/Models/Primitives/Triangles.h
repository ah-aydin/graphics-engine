#pragma once

#include <Engine/glApi/Rendering/Models/GLMesh2D.h>

#include <memory>

#ifdef GRAPHICS_API_OPENGL

namespace kbb::glApi::primitives::mesh
{
	std::shared_ptr<GLMesh2D> triangle2D();
}

#endif