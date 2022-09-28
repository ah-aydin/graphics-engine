#pragma once

#ifdef GRAPHICS_API_OPENGL

#include <Engine/Level/Level.h>
#include <Engine/glApi/Rendering/Shaders/GLProgram.h>

#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace kbb::glApi
{
	struct PerFrameData
	{
		glm::mat4 model;
		glm::mat4 mvp;
		glm::vec4 cameraPos;
	};
	 
	class LevelCubemap : public Level
	{
	public:
		LevelCubemap();
		void update(double dt) override;
		~LevelCubemap();
	private:
		GLuint vao = 0, dataVertices = 0, dataIndices = 0, txDuck = 0, perFrameDataBuffer = 0, txCubemap = 0;

		GLProgram<PerFrameData>* programDuck = nullptr, * programCube = nullptr;

		std::vector<unsigned int> indices;
	};
}
#endif