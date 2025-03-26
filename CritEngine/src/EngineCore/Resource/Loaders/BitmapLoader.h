#pragma once

#include "../ResourceLoader.h"
#include "../Resources/Image.h"
#include "../FileAccessor.h"

namespace Engine {

	//TODO: Add color palette support so that we can parse lower bpp formats.

	class BitmapLoader : public ResourceLoader
	{
	public:
		virtual ~BitmapLoader() override {};

		virtual std::string GetResourceType() override;
		virtual std::shared_ptr<Resource> Load(std::filesystem::path filepath) override;		
		

	};

}