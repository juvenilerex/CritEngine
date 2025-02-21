#pragma once

#include <iostream>
#include <cstdio>
#include <iomanip>
#include <string>
#include <chrono>
#include <sstream>

#include "../Core/Base.h"

// Functions to call a specific type of log
void ENGINE_API LogInfo(const std::string label, const std::string message);
void ENGINE_API LogWarning(const std::string label, const std::string message);
void ENGINE_API LogError(const std::string label, const std::string message);
