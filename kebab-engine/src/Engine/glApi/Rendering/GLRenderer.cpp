#include "GLRenderer.h"

#include <Engine/Settings.h>

#include "Shaders/UniformBufferObjects/ubo.h"

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
		m_program2D = new GLProgram<PerFrameDataObject2D, PerObjectDataObject2D>(vertex2D, fragment2D);

		// 3D program
		GLShader vertex3D("res/opengl/shader3D.vert");
		GLShader fragment3D("res/opengl/shader3D.frag");
		m_program3D = new GLProgram<PerFrameDataObject3D, PerObjectDataObject3D>(vertex3D, fragment3D);
	}

	void GLRenderer::render2D(std::vector<GLGameObject2D>& gameObjects)
	{
		m_program2D->use();
		
		PerFrameDataObject2D perFrameData;
		perFrameData.proj = glm::ortho(0.f, 800.f, 0.f, 600.f, 0.1f, 100.f);
		perFrameData.view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		
		m_program2D->setPerFrameData(perFrameData);

		for (auto& obj : gameObjects)
		{
			PerObjectDataObject2D perObjectData;
			perObjectData.model = obj.m_transform.mat2();
			perObjectData.offset = obj.m_transform.translation;
			
			m_program2D->setPerObjectData(perObjectData);

			obj.m_model->draw();
		}
	}

	void GLRenderer::render3D(std::vector<GLGameObject3D>& gameObjects)
	{
		m_program3D->use();

		for (auto& obj : gameObjects)
		{
			obj.m_model->draw();
		}
	}
}

#endif