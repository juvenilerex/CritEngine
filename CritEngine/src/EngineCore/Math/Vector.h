#pragma once
#include <string>
#include <sstream>

struct Vector2 {
	float x, y;

	constexpr Vector2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

	// Overloading operators and defining custom behavior for these math operations
	// (Will finish this soon, just getting this for mouse position stuff at the moment)
	constexpr Vector2 operator+(const Vector2& other) const {
		return Vector2(x + other.x, y + other.y);
	}

	constexpr Vector2 operator-(const Vector2& other) const {
		return Vector2(x - other.x, y - other.y);
	}

	constexpr float GetX() const {
		return x;
	}

	constexpr float GetY() const {
		return y;
	}

	std::string ToString() const {
		return "(" + std::to_string(x) + "," + std::to_string(y) + ")";
	}

};