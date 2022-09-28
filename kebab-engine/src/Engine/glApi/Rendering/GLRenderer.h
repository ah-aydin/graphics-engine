#pragma once

#include "Shaders/GLShader.h"
#include "Shaders/GLProgram.h"
#include "Shaders/PerFrameData/PerFrameData2D.h"
#include "Shaders/PerFrameData/PerFrameData3D.h"

#include <Engine/glApi/GameObject/GLGameObject2D.h>
#include <Engine/glApi/GameObject/GLGameObject3D.h>

#ifdef GRAPHICS_API_OPENGL

namespace kbb::glApi
{
	class GLRenderer
	{
	public:
		GLRenderer();
		~GLRenderer();

		GLRenderer(const GLRenderer&) = delete;
		GLRenderer& operator=(const GLRenderer&) = delete;

		void render2D(std::vector<GLGameObject2D>& gameObjects);
		void render3D(std::vector<GLGameObject3D>& gameObjects);

	private:
		void createShaderPrograms();

		GLProgram<PerFrameData2D>* m_program2D;
		GLProgram<PerFrameData3D>* m_program3D;
	};
}

#endif
