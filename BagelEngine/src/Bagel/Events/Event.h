#pragma once

#include "Bagel/Core.h"

#include "bgpch.h"

namespace Bagel {
	//Events are currently blocking, meaning that whenever an event occurs the rest of the application is halted until the event is dispatched and handled.
	//TODO: For future use a better system will be to create a event queue/buffer where all events are dispatched once per frame

	//Holds each of the possible event types. This list will be expanded as more events are needed, for example handling joysticks
	//Each of these events are handled in their respective files. For example KeyPressed is handled in KeyEvents.h etc.
	//TODO: Expand this to include Joysticks etc (Also TODO: Research how joysticks are typically handled)
	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	//We may want to filter certain event categories in the future, for example logging every keyboard event per frame, every mouse event per frame etc. 
	//Uses Bit fields to 1 shifted by N places. BIT(0) = 0x0000, BIT(1) = 0x0010 etc. This is used to query if an event is inside a certain category
	enum EventCategory {
		None = 0,
		EventCategoryApplication =	BIT(0),
		EventCategoryInput =		BIT(1),
		EventCategoryKeyboard =		BIT(2),
		EventCategoryMouse =		BIT(3),
		EventCategoryMouseButton =	BIT(4)
	};

	//This macro creates all the overrides for each of the classes that derive from the the event class. Simplifies the code creation of each class
#define EVENT_CLASS_TYPE(type)	static EventType GetStaticType() { return EventType::##type;}\
								virtual EventType GetEventType() const override { return GetStaticType();}\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override {return category; }
	//Base class for all events
	class BG_API Event {
	public:
		bool Handled = false;

		//Virtual methods. Creation handled by the EVENT_CLASS_TYPE and EVENT_CLASS_CATEGORY macros.
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;

		//This method is marked as virtual to so we can give more information to each type of event.
		//For example a MouseButtonPressed event may want to say what button was pressed and at what position.
		//To do this override this method and add in the additional data.
		virtual std::string ToString() const { return GetName(); }

		//Uses bit fields to ask if this event is in the category. 
		inline bool IsInCategory(EventCategory category) {
			return GetCategoryFlags() & category; //Returns 0 if not in category, returns anything else if it is in at least that category (may be in other categories as well)
		}

	};

	//Dispatches events based on their type
	class EventDispatcher {
		//T should be a Event type
		template<typename T>
		using EventFn = std::function<bool(T&)>;

	public:
		//We recieve a event reference meaning we do not know the type
		EventDispatcher(Event& event) : _event(event) {}

		//This template allows us to pass through an event handler so long as it returns a bool
		//T can be any everny type
		template<typename T>
		bool Dispatch(EventFn<T> func) {
			//If the event type of the methods paramater matches the _event member variables type then lets handle that event
			if (_event.GetEventType() == T::GetStaticType()) {
				//*(T*)&_event
				//Order of operations
				//&_event gets the memory address of the event (now Event*)
				//Event* gets casted to (T*)
				//The now T* gets dereferenced and passed through to the function. Passed in.

				_event.Handled = func(*(T*)&_event); //Dispatches the event to the appropriate handler
				return true;
			}

			return false;
		}

	private:
		Event& _event;
	};
	
	//Simplifies how we can log event information to the client
	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.ToString();
	}

}