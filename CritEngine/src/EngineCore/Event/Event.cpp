#pragma once

#include "../Logging/Logger.h"
#include "Event.h"

namespace Engine {

	EventEmitterBase::EventEmitterBase() {
		LogInfo("Event", "Emitter Created!");
	};

	void EventEmitterBase::AddListener(std::function<void()> func) {
		this->callbacks.push_back(func);
	}

	void EventEmitterBase::Emit() {

		for (int32_t i = 0; i < this->callbacks.size(); i++) {
			this->callbacks[i]();
		}
	}
}