#pragma once

#include "Event.h"

namespace Engine {

	class WindowResizeEvent : public Event {

	public:
		WindowResizeEvent(const int width, const int height) : width(width), height(height) {}

		std::string Print() const override {
			std::ostringstream ss;
			ss << "WindowResizeEvent: " << this->width << ", " << this->height;
			return ss.str();
		}

		int GetWidth() const { return this->width; }
		int GetHeight() const { return this->height; }


		SET_EVENT_TYPE(WindowResize);
		SET_EVENT_CATEGORY(EventWindow);

	private:
		int width, height;
	};

	class WindowCloseEvent : public Event {

	public:
		WindowCloseEvent() {}
		
		SET_EVENT_TYPE(WindowClose);
		SET_EVENT_CATEGORY(EventWindow);
	};
}