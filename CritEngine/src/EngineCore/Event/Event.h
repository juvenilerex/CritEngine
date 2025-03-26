#pragma once

#include <iostream>
#include <functional>

#include "../Core/Base.h"
#include "../Logging/Logger.h"

#define SET_EVENT_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
	virtual EventType GetEventType() const override { return GetStaticType(); }\
	virtual std::string GetName() const override { return std::string(#type); }

#define SET_EVENT_CATEGORY(cat) virtual int GetCategories() const override { return cat; }

namespace Engine {

	enum EventCategory {
		None = 0,
		EventApplication = 1 << 0,
		EventKeyboard = 1 << 1,
		EventMouse = 1 << 2,
		EventMouseButton = 1 << 3,
		EventInput = 1 << 4
	};

	enum class EventType {
		None = 0,
		WindowClose, WindowResize,
		WindowFocusLost, WindowFocus,
		WindowMoved,
		OnTick, OnRender,
		KeyPressed, KeyReleased, KeyJustPressed,
		MousePressed, MouseReleased, MouseMoved, MouseScroll
	};

	class Event {
	public:
		virtual ENGINE_API EventType GetEventType() const = 0;
		virtual ENGINE_API std::string GetName() const = 0;

		// Custom function for printing to the debug logger
		virtual ENGINE_API std::string Print() const = 0;

		virtual ENGINE_API int GetCategories() const = 0;

		void ENGINE_API SetHandled(const bool isHandled);
		bool ENGINE_API IsHandled() const;

		bool IsInCategory(EventCategory category)
		{
			return GetCategories() & category;
		}

	private:
		bool eventHandled = false;

	};

	class EventDispatcher {

		template<typename T>
		using EventFunc = std::function<bool(T&)>;
		
	public:
		EventDispatcher(Event& event) : event(event) {}

		template<typename T>
		bool Dispatch(EventFunc<T> func) {
			ASSERT(this->event.GetEventType() == T::GetStaticType(), "Can not mismatch event types");
			bool handled = func(static_cast<T&>(this->event));
			this->event.SetHandled(handled);
			return handled;
		}

	private:
		Event& event;

	};

}