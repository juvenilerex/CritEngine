#pragma once

namespace ECS {

    class Component {
    public:
        virtual ~Component() = default;

        inline void BindToEntity(const size_t ID) { this->boundEntityID = ID; }
        inline size_t GetEntity() const { return boundEntityID; }
    private:
        size_t boundEntityID;
    };

}