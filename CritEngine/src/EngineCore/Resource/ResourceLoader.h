#pragma once

namespace Engine {

	class ResourceLoader
	{
	public:
		virtual ~ResourceLoader() = 0;

		virtual std::string GetResourceType() = 0;
		virtual Resource Load(std::filesystem::path filepath) = 0;
	};

}