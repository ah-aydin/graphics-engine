#pragma once

#include <Engine/Level.h>
#include <vector>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <assimp/version.h>

#include "../Rendering/Shaders/GLProgram.h"
#include "../Rendering/Shaders/GLShader.h"

class LevelCubemap : public Level
{
public:
	LevelCubemap();
	bool init() override;
	void update(double dt) override;
	~LevelCubemap();

private:
	GLuint vao = 0, dataVertices = 0, dataIndices = 0, txDuck = 0, perFrameDataBuffer = 0, txCubemap = 0;

	GLShader* shaderDuckVert, * shaderDuckFrag, * shaderDuckGeom;
	GLShader* shaderCubeVert, * shaderCubeFrag;
	GLProgram* programDuck, * programCube;;

	std::vector<unsigned int> indices;

};