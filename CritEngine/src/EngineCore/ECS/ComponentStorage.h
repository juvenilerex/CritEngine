#pragma once

#include "PagedVector.h"

#include "Component.h"

namespace ECS {

	static const size_t PAGE_SIZE = 256;

	class ComponentStorage
	{
	public:
		
		template<typename T>
		void AddPool()
		{
			static_assert(std::is_base_of_v<Component, T>, "T must be a subclass of Component.");
			Engine::PagedVector<T, PAGE_SIZE> pool;
			
			this->componentPools.push_back(reinterpret_cast<Engine::PagedVector<Component, PAGE_SIZE>*>(&pool));
		}

	private:
		std::vector<Engine::PagedVector<Component, PAGE_SIZE>> componentPools;
	};

}