#include "kbbpch.h"
#include "Application.h"

#include "Input.h"

#include "Renderer/Renderer.h"
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

		renderer::Renderer::Create();
		renderer::Renderer::SetClearColor(1.f, 0.f, 1.f, 0.f);
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

			renderer::Renderer::BeginFrame();
			
			renderer::Renderer::EndFrame();

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
