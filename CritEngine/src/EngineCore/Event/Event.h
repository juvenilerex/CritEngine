#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <unordered_map>

#include "../Core.h"


namespace Engine {

	class ENGINE_API EventEmitter {
	public:

		EventEmitter();

		void AddListener(std::function<void()> func);
		void Emit();

	protected:
		std::vector<std::function<void()>> callbacks;
	};
};
