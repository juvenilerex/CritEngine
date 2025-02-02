#pragma once

#include "Window.h"
#include <unordered_map>

namespace Engine {

	class ENGINE_API InputListener {

	public:
		InputListener(Engine::Window& window);

		bool GetKeyDown(int key) const;
		bool GetKeyUp(int key) const;

		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	private:
		GLFWwindow* windowHandle;
		static std::unordered_map<int, bool> KeyStates;
		static std::unordered_map<int, bool> KeyReleased;

	};

}