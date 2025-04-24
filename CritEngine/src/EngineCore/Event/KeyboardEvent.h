#pragma once

#include "../Logging/Logger.h"
#include "Event.h"

namespace Engine {

	class KeyboardEvent : public Event {
	public:

		unsigned int GetKeyCode() const { return this->keyCode; }
		void SetKeyCode(unsigned int keycode) { this->keyCode = keycode; }
		
		SET_EVENT_TYPE(None);
		SET_EVENT_CATEGORY(EventKeyboard | EventInput);

	private:
		unsigned int keyCode;
	};

	class KeyPressedEvent : public KeyboardEvent {
	public:
		KeyPressedEvent(unsigned int keycode) { SetKeyCode(keycode); }

		void Print() const override {
			Debug::Log("KeyPressedEvent:", GetKeyCode());
		}

		SET_EVENT_TYPE(KeyPressed);
	};

	class KeyReleasedEvent : public KeyboardEvent {
	public:
		KeyReleasedEvent(unsigned int keycode) { SetKeyCode(keycode); }

		void Print() const override {
			Debug::Log("KeyReleasedEvent:", GetKeyCode());
		}

		SET_EVENT_TYPE(KeyReleased);
	};

	class KeyJustPressedEvent : public KeyboardEvent {
	public:
		KeyJustPressedEvent(unsigned int keycode) { SetKeyCode(keycode); }

		void Print() const override {
			Debug::Log("KeyJustPressedEvent:", GetKeyCode());
		}

		SET_EVENT_TYPE(KeyJustPressed);
	};
}
