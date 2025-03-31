#pragma once

#include <iostream>
#include <functional>

#include "../Core/Base.h"
#include "../Logging/Logger.h"

#define SET_EVENT_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
	virtual EventType GetEventType() const override { return GetStaticType(); }\
	virtual std::string GetName() const override { return std::string(#type); }

#define SET_EVENT_CATEGORY(cat) virtual int GetCategories() const override { return cat; }

#define BIND_EVENT_FUNC(func) [this] (auto& event) -> decltype(auto) { return func(event); }

namespace Engine {

	enum EventCategory {
		None = 0,
		EventKeyboard = 1 << 0,
		EventMouse = 1 << 1,
		EventInput = 1 << 2,
		EventWindow = 1 << 3
	};

	// Some of these may or may not be used in the future
	enum class EventType {
		None = 0,
		WindowClose, WindowResize,
		WindowFocusLost, WindowFocus,
		WindowMoved,
		OnTick, OnRender,
		KeyPressed, KeyReleased, KeyJustPressed,
		MousePressed, MouseReleased, MouseJustPressed, MouseMoved, MouseScroll
	};

	class Event {
	public:
		virtual ENGINE_API EventType GetEventType() const = 0;
		virtual ENGINE_API std::string GetName() const = 0;

		virtual ENGINE_API void Print() const {}

		virtual ENGINE_API int GetCategories() const = 0;

		void ENGINE_API SetHandled(const bool isHandled);
		bool ENGINE_API IsHandled() const;

		bool IsInCategory(const EventCategory category) const { return GetCategories() & category; }

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
			if (this->event.GetEventType() == T::GetStaticType()) {
				bool handled = func(static_cast<T&>(this->event));
				this->event.SetHandled(handled);
				return handled;

			}
		}

	private:
		Event& event;

	};

}