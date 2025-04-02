#include "Event.h"

namespace Engine {

	void Event::SetHandled(const bool isHandled)
	{
		this->eventHandled = isHandled;
	}

	bool Event::IsHandled() const
	{
		return this->eventHandled;
	}

}