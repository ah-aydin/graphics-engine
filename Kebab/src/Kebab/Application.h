#pragma once

#include <Kebab/Window.h>
#include <Kebab/Events/Event.h>

namespace kbb
{
	class Application
	{
	protected:
		Application();

	public:
		virtual ~Application();

		Application(const Application&) = delete;
		Application operator=(const Application&) = delete;

	public:
		void run();

		void onEvent(Event& e);

	private:
		std::unique_ptr<Window> m_window;
		
		bool m_running;
		void shutdown();
	};

	// To be defined in client
	Application* CreateApplication();
}
