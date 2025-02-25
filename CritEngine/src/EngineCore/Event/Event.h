#pragma once

#include <iostream>
#include <vector>
#include <functional>

#include "../Core/Base.h"

namespace Engine {

	// Variadic Templates only accept 1 .. n arguments, so we have an identical template for 0 arguments.
	template<typename... Ts>
	class EventEmitterBase
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
