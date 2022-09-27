#include "GLRenderer.h"

#ifdef GRAPHICS_API_OPENGL

namespace kbb::glApi
{
	GLRenderer::GLRenderer()
	{
		createShaderPrograms();
	}

	GLRenderer::~GLRenderer()
	{
		delete m_program2D;
		delete m_program3D;
	}

	void GLRenderer::createShaderPrograms()
	{
		// 2D program
		GLShader vertex2D("res/opengl/shader2D.vert");
		GLShader fragment2D("res/opengl/shader2D.frag");
		m_program2D = new GLProgram(vertex2D, fragment2D);

		// 3D program
		GLShader vertex3D("res/opengl/shader3D.vert");
		GLShader fragment3D("res/opengl/shader3D.frag");
		m_program3D = new GLProgram(vertex3D, fragment3D);
	}

	void GLRenderer::render2D(std::vector<GLGameObject2D>& gameObjects)
	{
		m_program2D->use();

		// TODO add rendering code
		for (auto& obj : gameObjects)
		{
			obj.m_model->draw();
		}
	}

	void GLRenderer::render3D(std::vector<GLGameObject3D>& gameObjects)
	{
		m_program3D->use();

		// TODO add rendering code
	}
}

#endif