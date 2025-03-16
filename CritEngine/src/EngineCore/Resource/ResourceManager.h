#pragma once

#include <memory>
#include <filesystem>
#include <unordered_map>

#include "Resource.h"
#include "ResourceLoader.h"

namespace Engine {

	class ResourceManager
	{
	public:
		void RegisterLoader(std::unique_ptr<ResourceLoader> loader);
		std::shared_ptr<Resource> GetFromPath(std::string resourceType, std::filesystem::path filepath);

	private:
		std::unordered_map<std::string, std::unique_ptr<ResourceLoader>> registeredLoaders;

		std::unordered_map<UUID, std::weak_ptr<Resource>> resource_cache;
		std::unordered_map<std::string, UUID> filename_uuid_map;
	};

}