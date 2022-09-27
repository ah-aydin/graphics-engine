#pragma once

#include "Shaders/GLShader.h"
#include "Shaders/GLProgram.h"

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

		// TODO add a mesh/gameobject class for opengl to pass in a vector of rendering arguments
		void render2D(std::vector<GLGameObject2D>& gameObjects);
		void render3D(std::vector<GLGameObject3D>& gameObjects);

	private:
		void createShaderPrograms();

		GLProgram* m_program2D;
		GLProgram* m_program3D;
	};
}

#endif
