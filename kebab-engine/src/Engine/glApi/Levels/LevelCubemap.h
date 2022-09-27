#pragma once

#ifdef GRAPHICS_API_OPENGL

#include <Engine/Level/Level.h>
#include <Engine/glApi/Rendering/Shaders/GLProgram.h>

#include <vector>

namespace kbb::glApi
{
	class LevelCubemap : public Level
	{
	public:
		LevelCubemap();
		void update(double dt) override;
		~LevelCubemap();
	private:
		GLuint vao = 0, dataVertices = 0, dataIndices = 0, txDuck = 0, perFrameDataBuffer = 0, txCubemap = 0;

		GLProgram* programDuck = nullptr, * programCube = nullptr;

		std::vector<unsigned int> indices;
	};
}
#endif