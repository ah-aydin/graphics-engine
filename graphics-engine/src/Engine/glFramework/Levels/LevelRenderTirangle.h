#pragma once

#include <memory>

#include <Engine/Level.h>

#include "../Rendering/Shaders/GLShader.h"
#include "../Rendering/Shaders/GLProgram.h"

class LevelRenderTriangle : public Level
{
public:
	LevelRenderTriangle();
	bool init() override;
	void update(float dt) override;
	~LevelRenderTriangle();

private:
	GLShader* shaderVertex;
	GLShader* shaderFragment;
	GLProgram* program;

	GLuint vao = 0;
};