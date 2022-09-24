#pragma once

#ifdef GRAPHICS_API_OPENGL

#include <memory>

#include <Engine/Level/Level.h>

#include <Engine/glApi/Rendering/Shaders/GLShader.h>
#include <Engine/glApi/Rendering/Shaders/GLProgram.h>

namespace glApi::levels
{
	using rendering::shaders::GLShader;
	using rendering::shaders::GLProgram;

	class LevelRenderTriangle : public Level
	{
	public:
		LevelRenderTriangle();
		void update(double dt) override;
		~LevelRenderTriangle();

	private:
		GLShader* shaderVertex;
		GLShader* shaderFragment;
		GLProgram* program;

		GLuint vao = 0;
	};
}
#endif