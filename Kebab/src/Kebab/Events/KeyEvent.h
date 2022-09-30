#pragma once

#include "Event.h"

#include <sstream>

namespace kbb
{
	class KeyEvent : public Event
	{
	public:
		int getKeyCode() const { return m_keyCode; }

		int getCategoryFlags() const override { return EventCategoryKeyboard | EventCategoryInput;  }

	protected:
		KeyEvent(const int keyCode) : m_keyCode(keyCode) {}
		int m_keyCode;
	};

	class KeyDownEvent : public KeyEvent
	{
	public:
		KeyDownEvent(const int keyCode, bool isRepeat = false)
			: KeyEvent(keyCode), m_isRepeat(isRepeat) {}

		bool isRepeat() const { return m_isRepeat; }

		EventType getType() const override { return EventType::KeyDown; }
		const char* getName() const override { return "KeyDownEvent"; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << getName()  << " - Key Code: " << m_keyCode << " - Is Repeat: " << m_isRepeat;
			return ss.str();
		}

	private:
		bool m_isRepeat;
	};

	class KeyUpEvent : public KeyEvent
	{
	public:
		KeyUpEvent(const int keyCode)
			: KeyEvent(keyCode) {} 

		EventType getType() const override { return EventType::KeyUp; }
		const char* getName() const override { return "KeyUpEvent"; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << getName() << " - Key Code: " << m_keyCode;
			return ss.str();
		}
	};
}
