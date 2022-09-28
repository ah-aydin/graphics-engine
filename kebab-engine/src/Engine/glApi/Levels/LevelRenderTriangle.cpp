#include "LevelRenderTirangle.h"

#ifdef GRAPHICS_API_OPENGLL

#include <Engine/glApi/Rendering/Shaders/GLShader.h>

namespace kbb::glApi
{
	LevelRenderTriangle::LevelRenderTriangle()
	{
		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);

		GLShader shaderVertex("res/shader.vert");
		GLShader shaderFragment("res/shader.frag");
		program = new GLProgram(shaderVertex, shaderFragment);
		program->use();
	}

	LevelRenderTriangle::~LevelRenderTriangle()
	{
		glDeleteVertexArrays(1, &vao);
		delete program;
	}

	void LevelRenderTriangle::update(double dt)
	{
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}

#endif