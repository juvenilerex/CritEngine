#pragma once

#include "../ResourceLoader.h"
#include "../Resources/Image.h"
#include "../FileAccessor.h"

namespace Engine {

	class BitmapLoader : public ResourceLoader
	{
	public:
		virtual ~BitmapLoader() override {};

		virtual std::string GetResourceType() override;
		virtual std::shared_ptr<Resource> Load(std::filesystem::path filepath) override;		
		

	};

}