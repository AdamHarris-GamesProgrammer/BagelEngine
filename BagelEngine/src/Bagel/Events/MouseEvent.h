#pragma once

#include "Event.h"

#include <sstream>

namespace Bagel {
	class BG_API MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(float x, float y) :
		_x(x), _y(y) {}
		
		inline float GetX() const { return _x; }
		inline float GetY() const { return _y; }
		
		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: " << _x << ", " << _y;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float _x, _y;
	};

	class BG_API MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(float xOffset, float yOffset) :
			_xOffset(xOffset), _yOffset(yOffset) {}

		inline float GetX() const { return _xOffset; }
		inline float GetY() const { return _yOffset; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << _xOffset << ", " << _yOffset;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float _xOffset, _yOffset;
	};

	//Base class for all MouseButtonEvents
	class BG_API MouseButtonEvent : public Event {
	public:
		MouseButtonEvent(int button) :
			_button(button) {}

		inline int GetMouseButton() const { return _button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	protected:
		//Holds the common data for mouse button events
		int _button;
	};

	class BG_API MouseButtonPressedEvent : public MouseButtonEvent {
		MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << _button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class BG_API MouseButtonReleasedEvent : public MouseButtonEvent {
		MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << _button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

}