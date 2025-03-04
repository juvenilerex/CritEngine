#pragma once
#include <string>
#include <cmath> // Will use this library for trigonometric functions later (?)

struct Vector2 {
	float x, y;

	constexpr Vector2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

	// Overloading operators and defining custom behavior for these math operations
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

	constexpr float Dot(const Vector2& other) const {
		return x * other.x + y * other.y;
	}

	inline float Magnitude() const {
		return std::sqrtf(x * x + y * y);
	}

	// Static since we can't pass in member variables
	static float Distance(const Vector2& a, const Vector2& b) {
		return (a - b).Magnitude();
	}

	inline Vector2 Normalized() const {
		float mag = Magnitude();
		return Vector2(x / mag, y / mag);
	}

	inline std::string ToString() const {
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

	constexpr float Dot(const Vector3& other) const {
		return x * other.x + y * other.y + z * other.z;
	}

	// For Vector3 only, this gets the cross product that is useful for calculating normals
	constexpr Vector3 Cross(const Vector3& other) const {
		return Vector3(
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x
		);
	}

	float Magnitude() const {
		return std::sqrtf(x * x + y * y + z * z);
	}

	inline Vector3 Normalized() const {
		float mag = Magnitude();
		return Vector3(x / mag, y / mag, z / mag);
	}

	inline std::string ToString() const {
		return "(" + std::to_string(x) + "," + std::to_string(y) + std::to_string(z) + ")";
	}

};