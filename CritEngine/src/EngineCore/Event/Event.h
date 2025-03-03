#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <functional>

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
			this->emitters.emplace(emitterName, emitter);
		}

		template <typename... Ts>
		void RemoveEmitter(const std::string& emitterName) {
			if (this->emitters.find(emitterName) == this->emitters.end()) {
				LogError("EventBus", "RemoveEmitter: " + emitterName + " was not found. Check spelling and ensure the emitter exists!");
				return;
			}
			this->emitters.erase(emitterName);
		}

		template <typename... Ts>
		void AddListener(const std::string& emitterName, std::function<void(Ts...)> func) {
			auto it = this->emitters.find(emitterName);
			if (it == this->emitters.end()) {
				LogError("EventBus", "AddListener: " + emitterName + " was not found. Check spelling and ensure the emitter exists!");
				return;
			}
			auto typedEmitter = static_cast<EventEmitterBase<Ts...>*>(it->second);
			typedEmitter->AddListener(func);
		}

		template <typename... Ts>
		void RemoveListener(const std::string& emitterName, std::function<void(Ts...)> func) {
			auto it = this->emitters.find(emitterName);
			if (it == this->emitters.end()) {
				LogError("EventBus", "RemoveListener: " + emitterName + " was not found. Check spelling and ensure the emitter exists!");
				return;
			}
			auto typedEmitter = static_cast<EventEmitterBase<Ts...>*>(it->second);
			typedEmitter->RemoveListener(func);
		}

		template <typename... Ts>
		void Emit(const std::string& emitterName, Ts... args) {
			auto it = this->emitters.find(emitterName);
			if (it == this->emitters.end()) {
				LogError("EventBus", "Emit: " + emitterName + " was not found. Check spelling and ensure the emitter exists!");
				return;
			}
			auto typedEmitter = static_cast<EventEmitterBase<Ts...>*>(it->second);
			typedEmitter->Emit(args...);
		}

	private:
		std::unordered_map<std::string, EventEmitterWrapper*> emitters;
	};

	// Variadic Templates only accept 1 .. n arguments, so we have an identical template for 0 arguments.
	template<typename... Ts>
	class EventEmitterBase : public EventEmitterWrapper
	{
	public:

		EventEmitterBase() {}

		void AddListener(std::function<void(Ts...)> func);
		void RemoveListener(std::function<void(Ts...)> func);

		void Emit(Ts... args);

	private:
		std::vector<std::function<void(Ts...)>> callbacks;
	};

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
	void EventEmitterBase<Ts...>::RemoveListener(std::function<void(Ts...)> func)
	{
		auto it = std::remove_if(callbacks.begin(), callbacks.end(), [&func](const std::function<void(Ts...)>& callback) {

			// Comparing the addresses of the underlying callable objects. target(), a method from std::function
			// returns a pointer to the underlying callable object
			// This is the only way I've found to reliably identify the function we wanna delete
			// .template tells the compiler that what follows callback is a function template, otherwise it will misinterpet the code
			// as a member function.. What a doozy 
			return callback.template target<void(Ts...)>() == func.template target<void(Ts...)>();

			});

		if (it != callbacks.end()) {
			callbacks.erase(it, callbacks.end());
		}

	}

	template<>
	inline void EventEmitterBase<>::RemoveListener(std::function<void(void)> func)
	{
		auto it = std::remove_if(callbacks.begin(), callbacks.end(), [&func](const std::function<void(void)>& callback) {

			return callback.template target<void()>() == func.template target<void()>();

			});

		if (it != callbacks.end()) {
			callbacks.erase(it, callbacks.end());
		}
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
