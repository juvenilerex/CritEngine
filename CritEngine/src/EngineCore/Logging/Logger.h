#pragma once

#include <iostream>
#include <cstdio>
#include <iomanip>
#include <string>
#include <chrono>
#include <sstream>

#include "../Core.h"

namespace Engine {
	// Labels
	enum Severity { ERROR, WARNING, INFO };

	std::string ENGINE_API GetCurrentTimestamp();

	void ENGINE_API Log(std::string timestamp, std::string label, Severity severity, std::string msg);

	// Functions to call a specific type of log
	void ENGINE_API LogInfo(const std::string label, const std::string message);
	void ENGINE_API LogWarning(const std::string label, const std::string message);
	void ENGINE_API LogError(const std::string label, const std::string message);

};
