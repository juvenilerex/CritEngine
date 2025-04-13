#include <chrono>

#include "../Logging/Logger.h"
#include "Profiler.h"

namespace Engine {

	// This is a very simple profiler that's split into two classes that can either be stopped manually, or
	// print results when it leaves the scope of a function. I provided macros in the header file to make this
	// even easier to setup.

	void Profile::Stop()
	{
		auto endTime = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
		Debug::Log(this->funcName, (float)duration/1000.f, "ms");
	}

	ScopedProfile::~ScopedProfile()
	{
		this->Stop();
	}

}
