#pragma once

#include "../Logging/Logger.h"
#include <chrono>

#ifdef CE_DEBUG
	#define CE_PROFILE_MANUAL(name) Engine::Profile _profilenow(name);
	#define CE_PROFILE_MANUAL_STOP _profilenow.Stop();
	#define CE_PROFILE_FUNC(name) Engine::ScopedProfile _scopedprofile(name);
#endif

namespace Engine {

	class Profile {
	public:
		Profile(const std::string& name) : funcName(name), startTime(std::chrono::high_resolution_clock::now()) {}

		void ENGINE_API Stop();

	private:
		bool isProfiling = false;
		std::string funcName;
		std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
	};

	class ScopedProfile : public Profile {
	public:
		ScopedProfile(const std::string& name) : Profile(name) {}
		ENGINE_API ~ScopedProfile();
	};

}
