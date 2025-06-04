#pragma once

namespace ECS {

    class Scene;

    class System {
    public:
        System() {};
        virtual ~System() {};

        void SetScene(Scene* scene)
        {
            this->scene = scene;
        }
        virtual void Update() = 0;

        Scene* scene = nullptr;
    };

}