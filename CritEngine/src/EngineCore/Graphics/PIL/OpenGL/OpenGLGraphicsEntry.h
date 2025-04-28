#pragma once 

#include "../GraphicsEntry.h"

namespace Engine {

	class OpenGLGraphicsEntry : public GraphicsEntry
	{

		OpenGLGraphicsEntry();
		~OpenGLGraphicsEntry();

		virtual void Init() override;

	};

}