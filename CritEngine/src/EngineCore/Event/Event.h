#pragma once

#include <iostream>
#include <vector>
#include <functional>

#include "../Core/Base.h"

namespace Engine {

	class EventEmitterBase {
	public:

		ENGINE_API EventEmitterBase();
		ENGINE_API void AddListener(std::function<void()> func);
		ENGINE_API void Emit();

	private:
		std::vector<std::function<void()>> callbacks;
	};
};
