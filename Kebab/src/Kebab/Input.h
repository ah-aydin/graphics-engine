#pragma once

#include <Kebab/Core.h>
#include <Kebab/Events/KeyEvent.h>

#include <map>
#include <cstdint>
#include <string>
#include <functional>
#include <vector>

namespace kbb
{
	class Input
	{
	private:
		using KeyDownEventSubscriber= std::function<void(KeyDownEvent& e)>;
		using KeyUpEventSubscriber = std::function<void(KeyUpEvent& e)>;

		struct Axis
		{
			uint32_t positiveKeyCode;
			uint32_t negativeKeyCode;
			bool positiveKeyPressed;
			bool negativeKeyPressed;

			explicit Axis(uint32_t positiveKeyCode, uint32_t negativeKeyCode)
				: positiveKeyCode(positiveKeyCode), negativeKeyCode(negativeKeyCode), positiveKeyPressed(false), negativeKeyPressed(false)
			{}

			void handleKeyDownEvent(KeyDownEvent& e);
			void handleKeyUpEvent(KeyUpEvent& e);
		};

		struct Action
		{
			uint32_t keyCode;
			bool isActive;

			explicit Action(uint32_t keyCode)
				: keyCode(keyCode), isActive(false) {}

			void handleKeyDownEvent(KeyDownEvent& e);
			void handleKeyUpEvent(KeyUpEvent& e);
		};

	public:
		using AxisMapPair = std::pair<const std::string, Axis>;
		using AxisMapType = std::map<const std::string, Axis>;
		using ActionMapPair = std::pair<const std::string, Action>;
		using ActionMapType = std::map<const std::string, Action>;

	public:
		static float getAxis(const std::string& name);
		static bool getAction(const std::string& name);

		static bool createAxis(const std::string& name, uint32_t positiveKeyCode, uint32_t negativeKeyCode);
		static bool createAction(const std::string& name, uint32_t keyCode);

		static void handleKeyEvent(Event& e);

	private:
		static AxisMapType s_axes;
		static ActionMapType s_actions;

		static std::vector<KeyDownEventSubscriber> s_keyDownEventSubscribers;
		static std::vector<KeyUpEventSubscriber> s_keyUpEventSubscribers;
	};
}