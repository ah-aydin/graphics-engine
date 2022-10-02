#include "kbbpch.h"
#include "Application.h"

#include "Input.h"

#include "Renderer/GraphicsContext.h"
#include "Renderer/VertexArray.h"
#include <Graphics/OpenGL/Shaders/OpenGLProgram.h>

namespace kbb
{
	Application::Application()
		: m_running(true)
	{
		WindowProps windowProps{};
#ifdef _DEBUG
		windowProps.debug = true;
#endif
		m_window = std::unique_ptr<Window>(Window::create(windowProps));
		m_window->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));
	}

	Application::~Application()
	{
	}

	void Application::run()
	{
		auto program = renderer::OpenGLProgram("res/opengl/shader.vert", "res/opengl/shader.frag");

		auto layout = renderer::VertexBufferLayout({
			{ renderer::VertexBufferType::Float3, "aPos"},
			{ renderer::VertexBufferType::Float3, "aColor"}
		});
		std::vector<float> vertices = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
		};
		std::vector<uint32_t> indices{ 0, 1, 2 };

		std::shared_ptr<renderer::VertexArray> vertexArray = renderer::VertexArray::Create(
			vertices,
			std::make_optional<renderer::VertexBufferLayout>(layout),
			indices
		);

		while (m_running)
		{
			if (Input::getAction("QUIT"))
			{
				shutdown();
			}

			renderer::GraphicsContext::BeginFrame();

			program.use();
			vertexArray->bind();
			glDrawElements(GL_TRIANGLES, (GLsizei) vertexArray->getIndexCount(), GL_UNSIGNED_INT, 0);
			
			renderer::GraphicsContext::EndFrame();

			m_window->update();
		}

		Window::closeWindow(m_window.get());
	}

	void Application::shutdown()
	{
		m_running = false;
	}

	void Application::onEvent(Event& e)
	{
		if (e.isInCategory(EventCategoryKeyboard))
		{
			Input::handleKeyEvent(e);
		}
		if (e.isInCategory(EventCategoryWindow))
		{
			if (e.getType() == EventType::WindowClose)
			{
				shutdown();
			}
		}
	}
}
