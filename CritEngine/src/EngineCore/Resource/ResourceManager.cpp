#include "ResourceManager.h"

namespace Engine {

	ResourceManager::ResourceManager()
	{
	}

	void ResourceManager::RegisterLoader(std::unique_ptr<ResourceLoader> loader)
	{
		this->registeredLoaders[loader->GetResourceType()] = std::move(loader);
	}
	
	std::shared_ptr<Resource> ResourceManager::GetFromPath(std::string resourceType, std::filesystem::path filepath)
	{
		if (auto uuidEntry = this->filename_uuid_map.find(filepath.u8string()); uuidEntry != this->filename_uuid_map.end())
		{
			if (auto cacheEntry = this->resource_cache.find(uuidEntry->second); cacheEntry != this->resource_cache.end())
			{
				std::shared_ptr<Resource> asset = cacheEntry->second.lock();
				if (asset == nullptr)
				{
					// Cache entry exists, however it is no longer valid, so it is removed.
					this->filename_uuid_map.erase(filepath.u8string());
					this->resource_cache.erase(uuidEntry->second);
				}
				else
				{
					// Cache entry exists and is valid!
					return asset;
				}
			}
		}

		std::shared_ptr<Resource> asset = this->registeredLoaders[resourceType]->Load(filepath);

		UUID id = UUID();

		this->filename_uuid_map.insert(std::make_pair(filepath.u8string(), id));
		this->resource_cache.insert(std::make_pair(id, std::weak_ptr<Resource>(asset)));
		
		

		return asset;
	}
}