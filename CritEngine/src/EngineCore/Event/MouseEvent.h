#pragma once

#include "../Logging/Logger.h"
#include "Event.h";

namespace Engine {

	class MouseEvent : public Event {
	public:

		unsigned int GetButton() const { return this->button; }
		void SetButton(unsigned int _button) { this->button = _button; }

		SET_EVENT_TYPE(None);
		SET_EVENT_CATEGORY(EventMouse);

	private:
		unsigned int button;
	};

	class MousePressedEvent : public MouseEvent {
	public:
		MousePressedEvent(unsigned int _button) { SetButton(_button); }

		void Print() const override {
			Debug::Log("MousePressedEvent:", GetButton());
		}

		SET_EVENT_TYPE(MousePressed);
	};

	class MouseReleasedEvent : public MouseEvent {
	public:
		MouseReleasedEvent(unsigned int _button) { SetButton(_button); }

		void Print() const override {
			Debug::Log("MouseReleasedEvent:", GetButton());
		}

		SET_EVENT_TYPE(MouseReleased);
	};

	class MouseJustPressedEvent : public MouseEvent {
	public:
		MouseJustPressedEvent(unsigned int _button) { SetButton(_button); }

		void Print() const override {
			Debug::Log("MouseJustPressedEvent:", GetButton());
		}

		SET_EVENT_TYPE(MouseJustPressed);
	};

	class MouseMovedEvent : public MouseEvent {
	public:
		MouseMovedEvent(const double xpos, const double ypos) { SetCurPos(Vector2(xpos, ypos)); }

		void Print() const override {
			Debug::Log("MouseMovedEvent:", this->position);
		}

		Vector2& GetCurPos() {
			return position;
		}

		void SetCurPos(const Vector2& curpos) {
			position = curpos;
		}

		SET_EVENT_TYPE(MouseMoved);
	private:
		Vector2 position;
	};

}