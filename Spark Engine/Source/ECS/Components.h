#pragma once
#include "../Core/framework.h"

namespace SparkEngine {
    struct TransformComponent {
        Float3 position{ 0,0,0 };
        Float4 rotation{ 0,0,0,1 };
        Float3 scale{ 1,1,1 };
    };

    struct CameraComponent {
        float fov{ 45.0f };
        float nearPlane{ 0.1f };
        float farPlane{ 1000.0f };
    };

    struct RenderComponent {
        std::string mesh;
        std::string material;
        bool visible{ true };
    };

    struct LightComponent {
        enum class Type { Directional, Point, Spot } type{ Type::Directional };
        Float3 color{ 1,1,1 };
        float intensity{ 1.0f };
        float range{ 10.0f };
    };

    struct PhysicsComponent {
        Float3 velocity{ 0,0,0 };
        Float3 acceleration{ 0,0,0 };
        float mass{ 1.0f };
        bool isStatic{ false };
    };
}
