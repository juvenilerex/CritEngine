#pragma once

#include <iostream>
#include <vector>
#include <functional>

#include "../Core.h"


namespace Engine {

	class ENGINE_API EventEmitterBase {
	public:

		EventEmitterBase();

		void AddListener(std::function<void()> func);
		void Emit();

	protected:
		std::vector<std::function<void()>> callbacks;
	};
};
