#pragma once

#include "../Core/Base.h"
#include <unordered_map>
#include <array>

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

// GLFW keycode wrapper here so we can use it in the sandbox
namespace Keys {

    enum Key : uint8_t {
        SPACE, APOSTROPHE, COMMA, MINUS, PERIOD, SLASH,
        DIGIT_0, DIGIT_1, DIGIT_2, DIGIT_3, DIGIT_4, DIGIT_5, DIGIT_6, DIGIT_7, DIGIT_8, DIGIT_9,
        SEMICOLON, EQUAL,
        A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        LEFT_BRACKET, BACKSLASH, RIGHT_BRACKET, GRAVE_ACCENT,
        WORLD_1, WORLD_2, ESCAPE, ENTER, TAB, BACKSPACE,
        INSERT, DELETE_KEY, RIGHT, LEFT, DOWN, UP,
        PAGE_UP, PAGE_DOWN, HOME, END,
        CAPS_LOCK, SCROLL_LOCK, NUM_LOCK, PRINT_SCREEN, PAUSE,
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, F25,
        KP_0, KP_1, KP_2, KP_3, KP_4, KP_5, KP_6, KP_7, KP_8, KP_9,
        KP_DECIMAL, KP_DIVIDE, KP_MULTIPLY, KP_SUBTRACT, KP_ADD, KP_ENTER, KP_EQUAL,
        LEFT_SHIFT, LEFT_CONTROL, LEFT_ALT, LEFT_SUPER,
        RIGHT_SHIFT, RIGHT_CONTROL, RIGHT_ALT, RIGHT_SUPER, MENU,
        LAST
    };

    constexpr std::array<uint16_t, LAST + 1> KeyMap = {
        32, 39, 44, 45, 46, 47,
        48, 49, 50, 51, 52, 53, 54, 55, 56, 57,
        59, 61,
        65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90,
        91, 92, 93, 96,
        161, 162, 256, 257, 258, 259,
        260, 261, 262, 263, 264, 265,
        266, 267, 268, 269,
        280, 281, 282, 283, 284,
        290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 300, 301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314,
        320, 321, 322, 323, 324, 325, 326, 327, 328, 329,
        330, 331, 332, 333, 334, 335, 336,
        340, 341, 342, 343,
        344, 345, 346, 347, 348
    };

}

namespace Engine {

    constexpr uint16_t GetKeyCode(Keys::Key key) {
        return Keys::KeyMap[key];
    }

}