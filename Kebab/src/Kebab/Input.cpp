#include "kbbpch.h"
#include "Input.h"

namespace kbb
{
	void Input::Axis::handleKeyDownEvent(KeyDownEvent& e)
	{
		if (e.isRepeat()) return;

		if (this->negativeKeyCode == e.getKeyCode())
		{
			this->negativeKeyPressed = true;
		}
		else if (this->positiveKeyCode == e.getKeyCode())
		{
			this->positiveKeyPressed = true;
		}
	}

	void Input::Axis::handleKeyUpEvent(KeyUpEvent& e)
	{
		if (this->negativeKeyCode == e.getKeyCode())
		{
			this->negativeKeyPressed = false;
		}
		else if (this->positiveKeyCode == e.getKeyCode())
		{
			this->positiveKeyPressed = false;
		}
	}
	
	void Input::Action::handleKeyDownEvent(KeyDownEvent& e)
	{
		if (this->keyCode != e.getKeyCode() || e.isRepeat()) return;

		this->isActive = true;
	}

	void Input::Action::handleKeyUpEvent(KeyUpEvent& e)
	{
		if (this->keyCode != e.getKeyCode()) return;

		this->isActive = false;
	}

	Input::AxisMapType Input::s_axes;
	Input::ActionMapType Input::s_actions;

	std::vector<Input::KeyDownEventSubscriber> Input::s_keyDownEventSubscribers;
	std::vector<Input::KeyUpEventSubscriber> Input::s_keyUpEventSubscribers;

	float Input::getAxis(const std::string& name)
	{
		try
		{
			auto& axis = s_axes.at(name);
			bool pInput = axis.positiveKeyPressed;
			bool nInput = axis.negativeKeyPressed;

			if (pInput && nInput)	return 0.0f;
			if (pInput)				return 1.0f;
			if (nInput)				return -1.0f;
			return 0.0f;
		}
		catch (const std::exception& e)
		{
			KBB_CORE_ERROR("Invalid axis name {0}", name);
			KBB_CORE_ERROR("{0}", e.what());
			return 0.0f;
		}
		return 0.0f;
	}

	bool Input::getAction(const std::string& name)
	{
		try
		{
			auto& action = s_actions.at(name);
			return action.isActive;
		}
		catch (const std::exception& e)
		{
			KBB_CORE_ERROR("Invalid action name {0}", name);
			KBB_CORE_ERROR("{0}", e.what());
			return false;
		}
		return false;
	}

	bool Input::createAxis(const std::string& name, uint32_t positiveKeyCode, uint32_t negativeKeyCode)
	{
		KBB_CORE_INFO("Creating axis {0} - Positive Key {1} - Negative Key {2}", name, positiveKeyCode, negativeKeyCode);
		s_axes.insert(
			AxisMapPair(name, Axis(positiveKeyCode, negativeKeyCode))
		);

		// Subscribe to events
		auto& axis = s_axes.at(name);
		s_keyDownEventSubscribers.push_back(std::bind(&Axis::handleKeyDownEvent, &axis, std::placeholders::_1));
		s_keyUpEventSubscribers.push_back(std::bind(&Axis::handleKeyUpEvent, &axis, std::placeholders::_1));

		return true;
	}
	
	bool Input::createAction(const std::string& name, uint32_t keyCode)
	{
		KBB_CORE_INFO("Creaing action {0} - KeyCode {1}", name, keyCode);
		s_actions.insert(
			ActionMapPair(name, Action(keyCode))
		);

		// Subscribe to events
		auto& action = s_actions.at(name);
		s_keyDownEventSubscribers.push_back(std::bind(&Action::handleKeyDownEvent, &action, std::placeholders::_1));
		s_keyUpEventSubscribers.push_back(std::bind(&Action::handleKeyUpEvent, &action, std::placeholders::_1));

		return true;
	}

	void Input::handleKeyEvent(Event& e)
	{
		if (e.getType() == EventType::KeyDown)
		{
			for (auto& subscriber : s_keyDownEventSubscribers)
			{
				subscriber(static_cast<KeyDownEvent&>(e));
			}
		}
		else if (e.getType() == EventType::KeyUp)
		{
			for (auto& subscriber : s_keyUpEventSubscribers)
			{
				subscriber(static_cast<KeyUpEvent&>(e));
			}
		}
	}
}