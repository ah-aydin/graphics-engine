#pragma once

#ifdef GRAPHICS_API_OPENGL

#include <Engine/Level/Level.h>
#include <Engine/glApi/Rendering/Shaders/GLShader.h>
#include <Engine/glApi/Rendering/Shaders/GLProgram.h>

#include <vector>

class LevelCubemap : public Level
{
public:
	LevelCubemap();
	void update(double dt) override;
	~LevelCubemap();
private:
	GLuint vao = 0, dataVertices = 0, dataIndices = 0, txDuck = 0, perFrameDataBuffer = 0, txCubemap = 0;

	GLShader* shaderDuckVert, *shaderDuckGeom, *shaderDuckFrag;
	GLShader* shaderCubeVert, *shaderCubeFrag;
	GLProgram* programDuck, *programCube;;

	std::vector<unsigned int> indices;
};

#endif