#pragma once

#include "../Logging/Logger.h"
#include "Event.h"

namespace Engine {

	EventEmitter::EventEmitter() {
		LogWarning("Event", "Emitter Created!");
	};

	void EventEmitter::AddListener(std::function<void()> func) {
		this->callbacks.push_back(func);
	}

	void EventEmitter::Emit() {

		for (int32_t i = 0; i < this->callbacks.size(); i++) {
			this->callbacks[i]();
		}
	}
}