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
		while (m_running)
		{
			if (Input::getAction("QUIT"))
			{
				shutdown();
			}

			renderer::GraphicsContext::BeginFrame();
			
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
