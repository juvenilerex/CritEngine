#pragma once

#include "../Core/Base.h"
#include <unordered_map>

struct GLFWwindow;

namespace Engine {

	class InputListener {

	public:
		ENGINE_API InputListener(GLFWwindow* windowHandle);

		ENGINE_API bool GetKeyJustPressed(int key) const;
		ENGINE_API bool GetKeyDown(int key) const;
		ENGINE_API bool GetKeyUp(int key) const;

		ENGINE_API static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	private:
		GLFWwindow* windowHandle;
		static std::unordered_map<int, bool> KeyStates;
		static std::unordered_map<int, bool> KeyReleased;

	};

}