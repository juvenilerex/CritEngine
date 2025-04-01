#pragma once

namespace Engine { class ECSManager; }

namespace ECS {

    class System {
    public:
        System(Engine::ECSManager& manager) : manager(manager) {}

        virtual ~System() = default;
        virtual void Update() = 0;

    protected:
        // For now systems have direct access to the manager, which is a tad dangerous, but dead simple
        Engine::ECSManager& manager;
    };

}