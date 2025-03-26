#pragma once

#include "Event.h"

namespace Engine {

	class Example : public Event {
	public:
		Example() {}

		void Hi() {
			Debug::Log("Hello from Example class");
		}

		std::string Print() const override {
			return "";
		}

		SET_EVENT_TYPE(KeyPressed);
		SET_EVENT_CATEGORY(EventInput);

	private:

	};

}
