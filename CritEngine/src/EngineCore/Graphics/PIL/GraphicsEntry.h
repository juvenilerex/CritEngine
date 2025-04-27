#pragma once

#include "../../Core/Base.h"

namespace Engine {

	class GraphicsEntry
	{

		static std::unique_ptr<GraphicsEntry> Create();

		virtual void Init() = 0;

	};

}