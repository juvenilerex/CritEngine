#pragma once
#include <string>

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

	// Multiplying two Vectors together
    constexpr Vector2 operator*(const Vector2& other) const {
        return Vector2(x * other.x, y * other.y);
    }

	// Support for scalar multiplication as well
	constexpr Vector2 operator*(float scalar) const {
		return Vector2(x * scalar, y * scalar);
	}

	constexpr Vector2 operator/(const Vector2& other) const {
		return Vector2(x / other.x, y / other.y);
	}

	constexpr Vector2 operator/(float scalar) const {
		return Vector2(x / scalar, y / scalar);
	}

	std::string ToString() const {
		return "(" + std::to_string(x) + "," + std::to_string(y) + ")";
	}

};

struct Vector3 {
    float x, y, z;

    constexpr Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}

    constexpr Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    constexpr Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

	constexpr float GetX() const {
		return x;
	}

	constexpr float GetY() const {
		return y;
	}

	constexpr float GetZ() const {
		return z;
	}

    constexpr Vector3 operator*(const Vector3& other) const {
        return Vector3(x * other.x, y * other.y, z * other.z);
    }

    constexpr Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

    constexpr Vector3 operator/(const Vector3& other) const {
        return Vector3(x / other.x, y / other.y, z / other.z);
    }

    constexpr Vector3 operator/(float scalar) const {
        return Vector3(x / scalar, y / scalar, z / scalar);
    }

	std::string ToString() const {
		return "(" + std::to_string(x) + "," + std::to_string(y) + std::to_string(z) + ")";
	}

};