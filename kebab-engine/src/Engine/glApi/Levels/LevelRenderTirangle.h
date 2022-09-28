#pragma once

#ifdef GRAPHICS_API_OPENGLL

#include <memory>

#include <Engine/Level/Level.h>

#include <Engine/glApi/Rendering/Shaders/GLProgram.h>

namespace kbb::glApi
{
	class LevelRenderTriangle : public Level
	{
	public:
		LevelRenderTriangle();
		void update(double dt) override;
		~LevelRenderTriangle();

	private:
		GLProgram* program;

		GLuint vao = 0;
	};
}
#endif