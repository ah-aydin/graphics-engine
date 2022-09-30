#pragma once

#include "Event.h"

#include <sstream>

namespace kbb
{
	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;
		
		EventType getType() const override { return EventType::WindowClose; }
		const char* getName() const override { return "WindowCloseEvent"; }
		int getCategoryFlags() const override { return EventCategoryWindow;  }

		std::string toString() const override
		{
			return getName();
		}
	};

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(int width, int height)
			: m_width(width), m_height(height) {} 

		int getWidth() const { return m_width; }
		int getHeight() const { return m_height; }

		EventType getType() const override { return EventType::WindowResize; }
		const char* getName() const override { return "WindowResizeEvent"; }
		int getCategoryFlags() const override { return EventCategoryWindow; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << getName() << " - new size (" << m_width << ", " << m_height << ")";
			return ss.str();
		}

	private:
		int m_width;
		int m_height;
	};

	class WindowFocusEvent : public Event
	{
	public:
		WindowFocusEvent(bool focused)
			: m_focused(focused) {}

		int getFocused() const { return m_focused; }

		EventType getType() const override { return EventType::WindowFocus; }
		const char* getName() const override { return "WindowFocusEvent"; }
		int getCategoryFlags() const override { return EventCategoryWindow; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << getName() << " - is focused: " << m_focused;
			return ss.str();
		}

	private:
		bool m_focused;
	};
}
