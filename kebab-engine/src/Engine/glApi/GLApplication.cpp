#include "GLApplication.h"

#ifdef GRAPHICS_API_OPENGL

#include <Logging/Log.h>

#include <Engine/Input.h>
#include <Engine/Time.h>

#include <stdexcept>

#include "Rendering/Models/GLMesh2D.h"
#include "GameObject/GLGameObject2D.h"

#include "Rendering/Models/Primitives/Triangles.h"
#include "Rendering/Models/Primitives/Quadrilaterlas.h"

#include "GameObject/Primitives/Quadrilaterlas.h"
#include "GameObject/Primitives/Triangles.h"

namespace kbb::glApi
{
	GLApplication::GLApplication() : Application()
	{
		glClearColor(0, 0, 0, 1);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		Input::createAction("QUIT", GLFW_KEY_ESCAPE, false);
	}

	GLApplication::~GLApplication()
	{
	}

	void GLApplication::run()
	{
		GLGameObject2D square = primitives::gameObject::square();
		square.m_transform.translation = { 300.0f, 300.0f };
		square.m_transform.scale = { 100.0f, 100.0f };

		GLGameObject2D triangle = primitives::gameObject::triangle();
		triangle.m_transform.translation = { 200.0f, 500.0f };
		triangle.m_transform.scale = { 50.0f, 50.0f };

		std::vector<GLGameObject2D> gameObjects2D;
		gameObjects2D.push_back(std::move(square));
		gameObjects2D.push_back(std::move(triangle));
		
		while (!m_window.shouldClose())
		{
			if (Input::getAction("QUIT"))
			{
				m_window.setShouldClose(true);
			}

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			m_renderer.render2D(gameObjects2D);

			glfwPollEvents();
			m_window.swapBuffer();
			Time::tick();
		}
	}
}
#endif