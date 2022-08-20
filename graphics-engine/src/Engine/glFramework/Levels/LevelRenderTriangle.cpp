#include "LevelRenderTirangle.h"

LevelRenderTriangle::LevelRenderTriangle() 
{
	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);
}

LevelRenderTriangle::~LevelRenderTriangle()
{
	glDeleteVertexArrays(1, &vao);
	delete shaderVertex;
	delete shaderFragment;
	delete program;
}

bool LevelRenderTriangle::init()
{
	Level::init();

	shaderVertex = new GLShader("res/shader.vert");
	shaderFragment = new GLShader("res/shader.frag");
	program = new GLProgram(*shaderVertex, *shaderFragment);
	program->use();

	return true;
}

void LevelRenderTriangle::update(float dt)
{
	Level::update(dt);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}