#include "Logger.h"

const void Log(const std::string& message)
{
	Debug::Log(message);
}

const void LogInfo(const std::string& label, const std::string& message)
{
	Debug::Log("[" + label + "]", message);
}

const void LogWarning(const std::string& label, const std::string& message)
{
	Debug::LogWarning("[" + label + "]", message);
}

const void LogError(const std::string& label, const std::string& message)
{
	Debug::LogError("[" + label + "]", message);
}
