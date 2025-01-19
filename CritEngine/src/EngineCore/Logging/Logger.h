#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <chrono>
#include <sstream>

namespace Logger {

	class Logger {

	public:

		// Labels
		enum Label { ERROR, WARNING, INFO };
		enum Severity { CRITICAL, NONCRITICAL };


		static void Log(Label label, Severity severity, std::string str) {
			std::cout << str << std::endl;
		}

		// Functions to call a specific type of log
		static void Info(const std::string message) { Log(INFO, NONCRITICAL, LabelToString(INFO) + message + SeverityToString(NONCRITICAL) + GetCurrentTimestamp()); }
		static void Warning(const std::string message) { Log(WARNING, NONCRITICAL, message + SeverityToString(NONCRITICAL) + GetCurrentTimestamp());}
		static void Error(const std::string message) { Log(ERROR, CRITICAL, LabelToString(ERROR) + message + SeverityToString(CRITICAL) + GetCurrentTimestamp());}

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

		static std::string SeverityToString(Severity severity) {

			switch (severity) {

				case CRITICAL: return " Severity: CRITICAL ";
				case NONCRITICAL: return " Severity: NONCRITICAL ";

			}
		}

		static std::string LabelToString(Label label) {

			switch (label) {

				case INFO: return "INFO: ";
				case WARNING: return "WARNING: ";
				case ERROR: return "ERROR: ";

				default: return "UNKNOWN: ";

			}
		}

	};
}