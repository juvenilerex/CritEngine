#pragma once

#include "../Core/Base.h"
#include <unordered_map>
#include "../Math/Vector.h"

struct GLFWwindow;

namespace Engine {

	class MouseInputListener {

	public:
		ENGINE_API MouseInputListener(GLFWwindow* windowHandle);

		ENGINE_API bool GetMouseJustPressed(int button) const;
		ENGINE_API bool GetMouseDown(int button) const;
		ENGINE_API bool GetMouseUp(int button) const;
		ENGINE_API Vector2 GetPosition() const;

		ENGINE_API static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		ENGINE_API static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);

	private:
		GLFWwindow* windowHandle;
		static std::unordered_map<int, bool> ButtonStates, ButtonReleased;
		static double posX;
		static double posY;

	};

}

// GLFW keycode wrapper here so we can use it in the sandbox
namespace GLFW {

	constexpr auto MOUSE_BUTTON_1 = 0;
	constexpr auto MOUSE_BUTTON_2 = 1;
	constexpr auto MOUSE_BUTTON_3 = 2;
	constexpr auto MOUSE_BUTTON_4 = 3;
	constexpr auto MOUSE_BUTTON_5 = 4;
	constexpr auto MOUSE_BUTTON_6 = 5;
	constexpr auto MOUSE_BUTTON_7 = 6;
	constexpr auto MOUSE_BUTTON_8 = 7;

}