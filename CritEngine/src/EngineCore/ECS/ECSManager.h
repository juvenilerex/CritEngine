#pragma once
#include "ECSCommon.h"

#include <unordered_map>
#include <algorithm>
#include <memory>

namespace Engine {

	class ECSManager {
	public:

		//FIXME[LOW]: This will cause issues once ID's overflow, which normally may never happen but programs running for long enough will face this issue.
		ECS::Entity AddEntity() {
			static uint32_t ID = 0;
			this->entities.push_back(ID);
			return ID++;
		}

		void DestroyEntity(const ECS::Entity ent) {
			// Destroy component along with entity. This is done with the Erase-remove idiom.
			this->components.erase(std::remove_if(this->components.begin(), this->components.end(), [ent](const std::unique_ptr<ECS::Component>& cmp) {

				return cmp->GetEntity() == ent;

			}), this->components.end());

			this->entities.erase(std::remove(this->entities.begin(), this->entities.end(), ent), this->entities.end());
		}

		template<typename T>
		T* AddComponent(const ECS::Entity ent) {
			auto newCmp = std::make_unique<T>();
			newCmp->BindToEntity(ent);
			auto rawPtr = newCmp.get();

			this->components.push_back(std::move(newCmp));

			return rawPtr;
		}

        template<typename T>
        T* GetComponent(const ECS::Entity ent) {
			for (const auto& cmp : this->components) {
				if (cmp->GetEntity() == ent)
					return dynamic_cast<T*>(cmp.get());
			}
			return nullptr;
        }

		template<typename T>
		const std::vector<T*>& GetAllComponents() {
			// This lets us reuse the same memory after every call inside a thread. This keeps us from having to redudantly allocate new std::vectors each frame
			// This likely will need to be tweaked when multi-threading is implemented though or just a better method (such as views)
			thread_local std::vector<T*> cache(512);
			cache.clear();

			for (const auto& cmp : components) {
				if (auto ptr = dynamic_cast<T*>(cmp.get()))
					cache.push_back(ptr);
			}
			return cache;
		}

		template<typename T>
		void RemoveComponent(const ECS::Entity ent) {

			this->components.erase(std::remove_if(this->components.begin(), this->components.end(), [ent](const std::unique_ptr<ECS::Component>& cmp) {

				return cmp->GetEntity() == ent && dynamic_cast<T*>(cmp.get()) != nullptr;

			}), this->components.end());

		}

		template<typename T>
		void RegisterSystem(ECSManager& manager) {
			auto system = std::make_unique<T>(manager);
			this->systems.push_back(std::move(system));
		}

		inline void UpdateSystems() {
			for (const auto& sys : this->systems)
				sys->Update();
		}

		inline size_t GetEntityCount() const { return this->entities.size(); }

	private:
		std::vector<std::unique_ptr<ECS::Component>> components;
		std::vector<ECS::Entity> entities;
		std::vector<std::unique_ptr<ECS::System>> systems;
		
	};

}