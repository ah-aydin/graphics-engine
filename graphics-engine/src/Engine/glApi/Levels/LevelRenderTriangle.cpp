#include "LevelRenderTirangle.h"

#ifdef GRAPHICS_API_OPENGL

LevelRenderTriangle::LevelRenderTriangle() 
{
	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);

	shaderVertex = new GLShader("res/shader.vert");
	shaderFragment = new GLShader("res/shader.frag");
	program = new GLProgram(*shaderVertex, *shaderFragment);
	program->use();
}

LevelRenderTriangle::~LevelRenderTriangle()
{
	glDeleteVertexArrays(1, &vao);
	delete shaderVertex;
	delete shaderFragment;
	delete program;
}

void LevelRenderTriangle::update(double dt)
{
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

#endif