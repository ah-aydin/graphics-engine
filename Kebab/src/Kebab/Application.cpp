#include "kbbpch.h"
#include "Application.h"

#include "Input.h"
#include "Renderer/RendererContext.h"

namespace kbb
{
	Application::Application()
		: m_running(true)
	{
		m_window = std::unique_ptr<Window>(Window::create());
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

			RendererContext::BeginFrame();

			m_window->update();

			RendererContext::EndFrame();
		}

		RendererContext::CleanUp();
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
