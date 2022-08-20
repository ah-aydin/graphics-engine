#pragma once

#include <Engine/Level.h>
#include <vector>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <assimp/version.h>

#include "../Rendering/Shaders/GLProgram.h"
#include "../Rendering/Shaders/GLShader.h"

class LevelVtxPulling : public Level
{
public:
	LevelVtxPulling();
	bool init() override;
	void update(float dt) override;
	~LevelVtxPulling();

private:
	GLuint vao = 0, dataVertices = 0, dataIndices = 0, tx = 0, perFrameDataBuffer = 0;

	GLShader* shaderVert;
	GLShader* shaderFrag;
	GLShader* shaderGeom;
	GLProgram* program;

	std::vector<unsigned int> indices;

};