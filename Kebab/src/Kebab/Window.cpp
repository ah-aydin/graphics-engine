#include "kbbpch.h"
#include "Window.h"

#include <Kebab/Events/KeyEvent.h>
#include <Kebab/Events/MouseEvent.h>
#include <Kebab/Events/WindowEvent.h>

namespace kbb
{
	uint32_t Window::s_WindowCount = 0;

	Window* Window::create(const WindowProps& props)
	{
		auto window = new Window(props);
		Window::s_WindowCount++;
		return window;
	}

	void Window::closeWindow(const Window* window)
	{
		Window::s_WindowCount--;
		glfwSetWindowShouldClose(window->m_window, GLFW_TRUE);
		glfwDestroyWindow(window->m_window);
		if (s_WindowCount == 0)
		{
			glfwTerminate();
		}
	}

	Window::Window(const WindowProps& props)
	{
		m_data.title = props.title;
		m_data.width = props.width;
		m_data.height = props.height;


		if (Window::s_WindowCount == 0)
		{
			KBB_CORE_INFO("Initializing GLFW");
			if (glfwInit() == GLFW_FALSE)
			{
				KBB_CORE_ERROR("Failed to initialize GLFW");
				throw std::exception("Failed to initialize GLFW");
			}
			Window::s_WindowCount += 1;
		}

		glfwSetErrorCallback([](int error, const char* description)
			{
				KBB_CORE_ERROR("GLFW error - {0}", description);
			}
		);

		KBB_CORE_INFO("Creating window {0} ({1}, {2})", m_data.title, m_data.width, m_data.height);
		// TODO adapt to rendering API
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef NDEBUG
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		m_data.width = mode->width;
		m_data.height = mode->height;
		m_window = glfwCreateWindow(mode->width, mode->height, m_data.title.c_str(), glfwGetPrimaryMonitor(), NULL);
#elif _DEBUG
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		m_window = glfwCreateWindow((int)m_data.width, (int)m_data.height, m_data.title.c_str(), NULL, NULL);
#endif
		if (!m_window)
		{
			KBB_CORE_ERROR("Failed to create windows window");
			throw std::exception("Failed to create windows window");
		}

		glfwSetWindowUserPointer(m_window, &m_data);
		glfwMakeContextCurrent(m_window);
		// TODO handle VSync via WindowProps
		glfwSwapInterval(1);
		glfwGetFramebufferSize(m_window, &m_data.framebufferWidth, &m_data.framebufferHeight);

		// Window events
		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.eventCallback(event);
			}
		);

		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
			{
				WindowData data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowResizeEvent event(width, height);
				data.eventCallback(event);
			}
		);

		glfwSetWindowFocusCallback(m_window, [](GLFWwindow* window, int focused)
			{
				WindowData data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowFocusEvent event(focused != 0);
				data.eventCallback(event);
			}
		);

		// Key events
		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				if (action == GLFW_PRESS)
				{
					KeyDownEvent event(key);
					data.eventCallback(event);
				}
				else if (action == GLFW_RELEASE)
				{
					KeyUpEvent event(key);
					data.eventCallback(event);
				}
				else if (action == GLFW_REPEAT)
				{
					KeyDownEvent event(key, true);
					data.eventCallback(event);
				}
			}
		);

		// Mouse events
		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseMoveEvent event(xPos, yPos);
				data.eventCallback(event);
			}
		);

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				if (action == GLFW_PRESS)
				{
					MouseButtonDownEvent event(button);
					data.eventCallback(event);
				}
				else if (action == GLFW_RELEASE)
				{	
					MouseButtonUpEvent event(button);
					data.eventCallback(event);
				}
			}
		);
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_window);
	}

	void Window::setEventCallback(const EventCallbackFn& callbackFn)
	{
		m_data.eventCallback = callbackFn;
	}

	void Window::update()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_window);
	}
}
