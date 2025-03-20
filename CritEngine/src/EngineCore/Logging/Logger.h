#pragma once

#include <iostream>
#include <cstdio>
#include <iomanip>
#include <string>
#include <chrono>
#include <sstream>
#include <utility>

#include "../Core/Base.h"

#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "../Math/Vector4.h"

// Improved logger class with automatic type deduction
class Debug {
public:
	template<typename... Args>
	static void Log(Args&&... params) {
		std::string str = Process(std::forward<Args>(params)...);
		printf("\033[%s;1;1m[%s] %s \033[0m\n", std::to_string(Color::WHITE).c_str(), GetCurrentTimestamp().c_str(), str.c_str());
	}

	template<typename... Args>
	static void LogWarning(Args&&... params) {
		std::string str = Process(std::forward<Args>(params)...);
		printf("\033[%s;1;1m[%s] %s \033[0m\n", std::to_string(Color::YELLOW).c_str(), GetCurrentTimestamp().c_str(), str.c_str());
	}

	template<typename... Args>
	static void LogError(Args&&... params) {
		std::string str = Process(std::forward<Args>(params)...);
		printf("\033[%s;1;1m[%s] %s \033[0m\n", std::to_string(Color::RED).c_str(), GetCurrentTimestamp().c_str(), str.c_str());
	}

private:
	enum Color { RED = 31, YELLOW = 33, WHITE = 37 };

	// This function converts arguments into strings using std::to_string, so all supported types
	// will automatically be converted without having to break their components down and/or use std::to_string
	template<typename T>
	static std::string Process(T&& arg) {

		if constexpr (std::is_same_v<std::decay_t<T>, int>) {
			return std::to_string(arg);
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, float>) {
			return std::to_string(arg);
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, double>) {
			return std::to_string(arg);
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, const char*>) {
			return std::string(arg);
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, std::string>) {
			return arg;
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, Engine::Vector2>) {
			return "(" + std::to_string(arg.x) + "," + std::to_string(arg.y) + ")";
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, Engine::Vector3>) {
			return "(" + std::to_string(arg.x) + "," + std::to_string(arg.y) + "," + std::to_string(arg.z) + ")";
		}
		else {
			ASSERT(always_false<T>::value, "Unsupported type passed to Logger");
		}
	}

	// Recursive processing of every argument
	template<typename T, typename... Args>
	static std::string Process(T&& first, Args&&... rest) {
		return Process(std::forward<T>(first)) + " " + Process(std::forward<Args>(rest)...);
	}

	template<typename T>
	struct always_false : std::false_type {};

	static std::string GetCurrentTimestamp()
	{
		auto now = std::chrono::system_clock::now();
		auto duration = now.time_since_epoch();

		auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
		auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration) - hours;
		auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration) - hours - minutes;
		auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration) - hours - minutes - seconds;

		std::time_t time_now = std::chrono::system_clock::to_time_t(now);

		// This is considered deprecated because it's not thread-safe, but the alternatives are not cross-platform and
		// I don't think we need multithreading for this, so we can ignore the warning
		std::tm local_time = *std::localtime(&time_now);

		/* Regardless, this is how it would be done safely in Windows:
		std::tm local_time;
		localtime_s(&local_time, &time_now);

		And for POSIX:
		std::tm local_time;
		localtime_r(&time_now, &local_time);

		*/
		std::ostringstream timestamp;
		timestamp << std::put_time(&local_time, "%H:%M:%S") << ":"
			<< std::setfill('0') << std::setw(3) << milliseconds.count();

		return timestamp.str();
	}
};

const void ENGINE_API Log(const std::string& message);
const void ENGINE_API LogInfo(const std::string& label, const std::string& message);
const void ENGINE_API LogWarning(const std::string& label, const std::string& message);
const void ENGINE_API LogError(const std::string& label, const std::string& message);