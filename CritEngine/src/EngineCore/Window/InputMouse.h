#pragma once

#include "../Core/Base.h"
#include <unordered_map>
#include <array>
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

namespace Mouse {

	enum Button : uint8_t {
		BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4, BUTTON_5, BUTTON_6, BUTTON_7, BUTTON_8
	};

	constexpr std::array<uint8_t, BUTTON_8 + 1> ButtonMap = {
		{ 0, 1, 2, 3, 4, 5, 6, 7 }
	};

}

namespace Engine {

	constexpr uint8_t GetMouseButton(Mouse::Button button) {
		return Mouse::ButtonMap[button];
	}

}