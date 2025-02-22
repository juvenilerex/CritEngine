#pragma once

#include "Core/Base.h"
#include "Event/Event.h"

namespace Engine {

	class Layer {

	public:
		ENGINE_API Layer(const std::string& name = "UnnamedLayer"); // UnnamedLayer will be the default name if none is assigned
		ENGINE_API virtual ~Layer(); // Virtual destructor we'll use to clean up memory since we're using some raw pointers (for simplicity-sake)

		ENGINE_API void Test();
//		ENGINE_API virtual void OnEvent(EventEmitterBase& event) {} 
		ENGINE_API virtual void OnAttach() {}
		ENGINE_API virtual void OnDetach() {}
		ENGINE_API virtual void OnUpdate() {}

	};
}
