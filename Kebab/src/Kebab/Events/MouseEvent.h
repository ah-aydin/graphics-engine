#pragma once

#include "Event.h"

#include <sstream>

namespace kbb
{
	class MouseMoveEvent: public Event
	{
	public:
		MouseMoveEvent(double xPos, double yPos)
			: m_xPos(xPos), m_yPos(yPos) {}

		EventType getType() const override { return EventType::MouseMove; }
		const char* getName() const override { return "MouseMoveEvent"; }
		int getCategoryFlags() const override { return EventCategoryMouse; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << getName() << " - Pos (" << m_xPos << ", " << m_yPos << ")";
			return ss.str();
		}

	private:
		double m_xPos;
		double m_yPos;
	};

	class MouseButtonEvent : public Event
	{
	public:
		int getKeyCode() const { return m_keyCode; }

		int getCategoryFlags() const override { return EventCategoryMouse | EventCategoryMouseButton; }

	protected:
		MouseButtonEvent(const int keyCode) : m_keyCode(keyCode) {}
		int m_keyCode;
	};

	class MouseButtonDownEvent : public MouseButtonEvent
	{
	public:
		MouseButtonDownEvent(const int keyCode)
			: MouseButtonEvent(keyCode) {}

		EventType getType() const override { return EventType::MouseButtonDown; }
		const char* getName() const override { return "MouseButtonDownEvent"; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << getName() << " - KeyCode " << m_keyCode;
			return ss.str();
		}
	};
	
	class MouseButtonUpEvent : public MouseButtonEvent
	{
	public:
		MouseButtonUpEvent(const int keyCode) : MouseButtonEvent(keyCode) {} 

		EventType getType() const override { return EventType::MouseButtonUp; }
		const char* getName() const override { return "MouseButtonUpEvent"; }
		
		std::string toString() const override
		{
			std::stringstream ss;
			ss << getName() << " - KeyCode " << m_keyCode;
			return ss.str();
		}
	};
}
