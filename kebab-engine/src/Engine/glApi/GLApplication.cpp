#include "GLApplication.h"

#ifdef GRAPHICS_API_OPENGL

#include <Logging/Log.h>

#include <Engine/Input.h>
#include <Engine/Time.h>

#include <stdexcept>

#include "Levels/LevelRenderTirangle.h"
#include "Levels/LevelCubemap.h"

#include "Rendering/Models/GLMesh2D.h"
#include "GameObject/GLGameObject2D.h"

namespace kbb::glApi
{
	GLApplication::GLApplication() : Application()
	{
		glClearColor(0, 0, 0, 1);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		Input::createAction("QUIT", GLFW_KEY_ESCAPE, false);
		Input::createAction("LEVEL 1", GLFW_KEY_1, false);
		Input::createAction("LEVEL 2", GLFW_KEY_2, false);
	}

	GLApplication::~GLApplication()
	{
	}

	void GLApplication::run()
	{
		int levelIndex = 1;
		Level* level = new LevelRenderTriangle();

		std::vector<Vertex2D> verticies = {
			{{0.0f, 0.5f}, {0.0f, 0.0f}},
			{{0.5f, 0.0f}, {0.0f, 0.0f}},
			{{-0.5f, 0.0f}, {0.0f, 0.0f}}
		};
		std::shared_ptr<GLMesh2D> mesh = std::make_shared<GLMesh2D>(verticies);
		GLGameObject2D gameObject = GLGameObject2D::createGameObject();
		gameObject.m_model = mesh;

		std::vector<GLGameObject2D> gameObjects2D;
		gameObjects2D.push_back(std::move(gameObject));

		while (!m_window.shouldClose())
		{
			if (Input::getAction("QUIT"))
			{
				m_window.setShouldClose(true);
			}

			if (Input::getAction("LEVEL 1"))
			{
				if (levelIndex != 1) { delete level; level = new LevelRenderTriangle(); levelIndex = 1; }
			}
			if (Input::getAction("LEVEL 2"))
			{
				if (levelIndex != 2) { delete level; level = new LevelCubemap(); levelIndex = 2; }
			}

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//level->update(Time::getDeltaTime());
			m_renderer.render2D(gameObjects2D);

			glfwPollEvents();
			m_window.swapBuffer();
			Time::tick();
		}

		delete level;
	}
}
#endif