#include "GLRenderer.h"

#include <Engine/Settings.h>

#include "Shaders/PerFrameData/PerFrameData2D.h"
#include "Shaders/PerFrameData/PerFrameData3D.h"

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
		m_program2D = new GLProgram<PerFrameData2D>(vertex2D, fragment2D);

		// 3D program
		GLShader vertex3D("res/opengl/shader3D.vert");
		GLShader fragment3D("res/opengl/shader3D.frag");
		m_program3D = new GLProgram<PerFrameData3D>(vertex3D, fragment3D);
	}

	void GLRenderer::render2D(std::vector<GLGameObject2D>& gameObjects)
	{
		m_program2D->use();
		
		for (auto& obj : gameObjects)
		{
			PerFrameData2D perFrameData;
			perFrameData.offset = obj.m_transform.translation;
			perFrameData.transform = obj.m_transform.mat2();

			m_program2D->setPerFrameData(perFrameData);

			obj.m_model->draw();
		}
	}

	void GLRenderer::render3D(std::vector<GLGameObject3D>& gameObjects)
	{
		m_program3D->use();

		for (auto& obj : gameObjects)
		{
			PerFrameData3D perFrameData;
			perFrameData.model = obj.m_transform.mat4();
			perFrameData.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			perFrameData.proj = glm::perspective(glm::radians(45.f), Settings::ratio, 0.1f, 100.f);

			m_program3D->setPerFrameData(perFrameData);

			obj.m_model->draw();
		}
	}
}

#endif