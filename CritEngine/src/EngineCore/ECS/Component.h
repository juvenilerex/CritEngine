#pragma once

namespace ECS {

    class Component {
    public:
        virtual ~Component() = default;

        inline void BindToEntity(const size_t ID) { this->bindedEntityID = ID; }
        inline size_t GetEntity() const { return bindedEntityID; }
    private:
        size_t bindedEntityID;
    };

}