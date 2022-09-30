#pragma once

#include <Kebab/Core.h>

namespace kbb
{
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus,
		KeyDown, KeyUp,
		MouseMove, MouseButtonDown, MouseButtonUp
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryWindow = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryInput = BIT(3),
		EventCategoryMouse = BIT(4),
		EventCategoryMouseButton = BIT(5)
	};

	class Event
	{
	public:
		virtual ~Event() = default;

		bool Handled = false;

		virtual EventType getType() const = 0;
		virtual const char* getName() const = 0;
		virtual int getCategoryFlags() const = 0;

		virtual std::string toString() const { return ""; };

		bool isInCategory(EventCategory category)
		{
			return getCategoryFlags() & category;
		}
	};
}
