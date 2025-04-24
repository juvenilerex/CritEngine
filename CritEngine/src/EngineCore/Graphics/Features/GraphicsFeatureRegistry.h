#pragma once

#include <array>
#include <unordered_map>
#include "GraphicsFeature.h"

namespace Engine {

	enum GraphicsFeatureType : uint8_t
	{
		VertexBuffer,
		IndexBuffer,
		SIZE
	};

	class GraphicsFeatureRegistry
	{
	public:
		GraphicsFeatureRegistry(std::unordered_map<GraphicsFeatureType, GraphicsFeature> features);

		GraphicsFeatureSupport GetFeatureSupport(GraphicsFeatureType type);

	private:
		std::array<GraphicsFeature, GraphicsFeatureType::SIZE> features;
	};

}