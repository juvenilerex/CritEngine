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
			ASSERT(!IsRegistered(emitterName), "Can not overwrite registered emitter");
			this->emitters.insert({emitterName, emitter});
		}

		void RemoveEmitter(const std::string& emitterName) {
			ASSERT(IsRegistered(emitterName), "Can not access unregistered emitter");
			this->emitters.erase(emitterName);
		}

		template <typename... Ts>
		void AddListener(const std::string& emitterName, std::function<void(Ts...)> func) {
			ASSERT(IsRegistered(emitterName), "Can not access unregistered emitter");
			auto tEmitter = static_cast<EventEmitterBase<Ts...>*>(GetEmitter(emitterName)->second);
			tEmitter->AddListener(func);
		}

		template <typename... Ts>
		void RemoveListener(const std::string& emitterName, std::function<void(Ts...)> func) {
			ASSERT(IsRegistered(emitterName), "Can not access unregistered emitter");
			auto tEmitter = static_cast<EventEmitterBase<Ts...>*>(GetEmitter(emitterName)->second);
			tEmitter->RemoveListener(func);
		}

		template <typename... Ts>
		void Emit(const std::string& emitterName, Ts... args) {
			ASSERT(IsRegistered(emitterName), "Can not access unregistered emitter");
			auto tEmitter = static_cast<EventEmitterBase<Ts...>*>(GetEmitter(emitterName)->second);
			tEmitter->Emit(args...);
		}

		void RemoveAllListeners(const std::string& emitterName) {
			ASSERT(IsRegistered(emitterName), "Can not access unregistered emitter");
			this->emitters[emitterName] = 0;
		}

		inline void RemoveAllEmitters() {
			this->emitters.clear();
		}


	private:

		inline auto GetEmitter(const std::string& emitterName) { return this->emitters.find(emitterName); }
		inline bool IsRegistered(const std::string& emitterName) { 
			if (this->emitters.find(emitterName) != this->emitters.end()) return true; else {
				return false;
			}
		}

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
		auto it = std::remove_if(this->callbacks.begin(), this->callbacks.end(), [&func](const std::function<void(Ts...)>& callback) {

			// Comparing the addresses of the underlying callable objects. target(), a method from std::function
			// returns a pointer to the underlying callable object
			// This is the only way I've found to reliably identify the function we wanna delete
			// .template tells the compiler that what follows callback is a function template, otherwise it will misinterpet the code
			// as a member function.. What a doozy 
			return callback.template target<void(Ts...)>() == func.template target<void(Ts...)>();

			});

		if (it != this->callbacks.end()) {
			this->callbacks.erase(it, this->callbacks.end());
		}

	}

	template<>
	inline void EventEmitterBase<>::RemoveListener(std::function<void(void)> func)
	{
		auto it = std::remove_if(this->callbacks.begin(), this->callbacks.end(), [&func](const std::function<void(void)>& callback) {

			return callback.template target<void()>() == func.template target<void()>();

			});

		if (it != this->callbacks.end()) {
			this->callbacks.erase(it, this->callbacks.end());
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
