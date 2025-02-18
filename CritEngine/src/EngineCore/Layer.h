#pragma once

#include "Core.h"
#include "Event/Event.h"

namespace Engine {

	class ENGINE_API Layer {

	public:
		Layer(const std::string& name = "UnnamedLayer"); // UnnamedLayer will be the default name if none is assigned
		virtual ~Layer(); // Virtual destructor we'll use to clean up memory since we're using some raw pointers (for simplicity-sake)

		void Test();
//		virtual void OnEvent(EventEmitterBase& event) {} 
		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}

	};
}
