#pragma once

#include <iostream>
#include <cstdio>
#include <iomanip>
#include <string>
#include <chrono>
#include <sstream>

namespace Engine {

	class Logger {

	public:

		// Labels
		enum Severity { ERROR, WARNING, INFO };

		static void Log(std::string timestamp, std::string label, Severity severity, std::string msg) {


			enum Color { RED = 31, YELLOW = 33, WHITE = 37 }; // Enum of relevant ascii codes for readability

			int color = 0; // Our ascii color code

			switch (severity) {
			case ERROR:
				color = Color::RED;
				break;
			case WARNING:
				color = Color::YELLOW;
				break;
			case INFO:
				color = Color::WHITE;
				break;
			}

			printf("\033[%s;1;1m[%s][%s] %s \033[0m\n", std::to_string(color).c_str(), timestamp.c_str(), label.c_str(), msg.c_str());

		}

		// Functions to call a specific type of log
		static void Info(const std::string label, const std::string message) { Log(GetCurrentTimestamp(), label, Severity::INFO, message); }
		static void Warning(const std::string label, const std::string message) { Log(GetCurrentTimestamp(), label, Severity::WARNING, message); }
		static void Error(const std::string label, const std::string message) { Log(GetCurrentTimestamp(), label, Severity::ERROR, message); }

	private:

		static std::string GetCurrentTimestamp() {
			// Get current time
			auto now = std::chrono::system_clock::now();
			auto duration = now.time_since_epoch();

			// Break down to HH:MM:SS:MS
			auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
			auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration) - hours;
			auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration) - hours - minutes;
			auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration) - hours - minutes - seconds;

			// Convert to time_t for hour/minute/second formatting
			std::time_t time_now = std::chrono::system_clock::to_time_t(now);
			std::tm local_time = *std::localtime(&time_now);

			// Format the timestamp 
			std::ostringstream timestamp;
			timestamp << std::put_time(&local_time, "%H:%M:%S") << ":"
				<< std::setfill('0') << std::setw(3) << milliseconds.count();

			return timestamp.str();
		}

		/*
		static std::string SeverityToString(Severity severity) {

			switch (severity) {

				case INFO: return "INFO ";
				case WARNING: return "WARN ";
				case ERROR: return "ERROR";

			}
		}
		*/

	};
}