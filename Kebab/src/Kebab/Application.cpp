#include "kbbpch.h"
#include "Application.h"

#include "Input.h"

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
			m_window->update();
		}
	}

	void Application::shutdown()
	{
		Window::closeWindow(m_window.get());
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
