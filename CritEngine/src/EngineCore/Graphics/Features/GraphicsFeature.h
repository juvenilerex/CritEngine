#pragma once

#include <stdint.h>

namespace Engine {

	enum GraphicsFeatureSupport : uint8_t
	{
		Unsupported,
		Emulated,
		Available
	};

	class GraphicsFeature
	{
	public:
		GraphicsFeature::GraphicsFeature(GraphicsFeatureSupport supportLevel);

		GraphicsFeatureSupport GetSupportLevel();
	private:
		GraphicsFeatureSupport supportLevel;
	};
}