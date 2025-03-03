#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <functional>
#include <any>

#include "../Core/Base.h"

namespace Engine {

	// Forward declaration that somehow is the glue of this
	template<typename... Ts>
	class EventEmitterBase;

	// Non-templated wrapper class that EventEmitterBase will use as its base class,
	// allowing any specialization to be stored in our bus
	class EventEmitterWrapper {
	public:
		virtual ~EventEmitterWrapper() = default;
	};

	class EventBus {

	public:
		template <typename... Ts>
		void AddEmitter(const std::string& emitterName, EventEmitterBase<Ts...>* emitter) {
			emitters[emitterName].push_back(emitter);
		}

		template <typename... Ts>
		void AddListener(const std::string& emitterName, std::function<void(Ts...)> func) {
			auto it = emitters.find(emitterName);
			if (it != emitters.end()) {
				for (auto& _emitter : it->second) {
					auto typedEmitter = static_cast<EventEmitterBase<Ts...>*>(_emitter);
					ASSERT(typedEmitter, "Can't add listener to an invalid EventEmitterBase");
					typedEmitter->AddListener(func);
				}
			}
		}

	private:
		std::unordered_map<std::string, std::vector<EventEmitterWrapper*>> emitters;
	};

	// Variadic Templates only accept 1 .. n arguments, so we have an identical template for 0 arguments.
	template<typename... Ts>
	class EventEmitterBase : public EventEmitterWrapper
	{
	public:

		EventEmitterBase() {}

		void AddListener(std::function<void(Ts...)> func);

		void Emit(Ts... args);

	private:
		std::vector<std::function<void(Ts...)>> callbacks;
	};

	// Impl

	template <typename... Ts>
	void EventEmitterBase<Ts...>::AddListener(std::function<void(Ts...)> func)
	{
		this->callbacks.push_back(func);
	}

	template<>
	inline void EventEmitterBase<>::AddListener(std::function<void(void)> func)
	{
		this->callbacks.push_back(func);
	}

	template <typename... Ts>
	void EventEmitterBase<Ts...>::Emit(Ts... args)
	{
		for (int32_t i = 0; i < this->callbacks.size(); i++)
		{
			this->callbacks[i](std::forward<Ts>(args)...);
		}
	}

	template<>
	inline void EventEmitterBase<>::Emit()
	{
		for (int32_t i = 0; i < this->callbacks.size(); i++)
		{
			this->callbacks[i]();
		}
	}

};
