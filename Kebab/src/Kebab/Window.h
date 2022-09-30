#pragma once

#include <Kebab/Core.h>
#include <Kebab/Events/Event.h>

#include <GLFW/glfw3.h>

namespace kbb
{
	struct WindowProps
	{
		std::string title;
		uint32_t width;
		uint32_t height;

		explicit WindowProps(const std::string title = "Kebab Engine", uint32_t width = 1280, uint32_t height = 800)
			: title(title), width(width), height(height)
		{
		}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		static uint32_t s_WindowCount;
	protected:
		Window(const WindowProps& props);
		
	public:
		~Window();

		void update();

		uint32_t getWidth() const;
		uint32_t getHeight() const;

		void setEventCallback(const EventCallbackFn& callbackFn);

		static Window* create(const WindowProps& props = WindowProps());
		static void closeWindow(const Window* window);

	private:
		struct WindowData
		{
			std::string title;
			uint32_t width;
			uint32_t height;
			int32_t framebufferWidth;
			int32_t framebufferHeight;
			EventCallbackFn eventCallback;
		};

		WindowData m_data;

		GLFWwindow* m_window;
	};
}
