#pragma once
#include "../Core/framework.h"

#ifdef ENTT_AVAILABLE
#include <entt/entt.hpp>
#endif

namespace SparkEngine {

    struct TransformComponent {
        XMFLOAT3 position{0,0,0};
        XMFLOAT4 rotation{0,0,0,1}; // quaternion
        XMFLOAT3 scale{1,1,1};
        XMMATRIX GetWorldMatrix() const {
            return XMMatrixScaling(scale.x,scale.y,scale.z)
                 * XMMatrixRotationQuaternion(XMLoadFloat4(&rotation))
                 * XMMatrixTranslation(position.x,position.y,position.z);
        }
    };

    struct CameraComponent {
        float fov{45}, nearPlane{0.1f}, farPlane{1000};
        float aspectRatio{16.0f/9.0f};
        XMMATRIX GetProjectionMatrix() const {
            return XMMatrixPerspectiveFovLH(XMConvertToRadians(fov),aspectRatio,nearPlane,farPlane);
        }
        XMMATRIX GetViewMatrix(const XMFLOAT3& pos,const XMFLOAT4& rot) const {
            XMVECTOR p=XMLoadFloat3(&pos), r=XMLoadFloat4(&rot);
            return XMMatrixInverse(nullptr,XMMatrixTranslationFromVector(p)*XMMatrixRotationQuaternion(r));
        }
    };

    struct RenderComponent {
        std::string meshName, materialName;
        bool visible{true};
        XMFLOAT4 color{1,1,1,1};
    };

    struct LightComponent {
        enum Type{Directional,Point,Spot} type{Directional};
        XMFLOAT3 color{1,1,1};
        float intensity{1}, range{10};
        XMFLOAT4 cursorColor{1,1,1,1};
    };

    struct PhysicsComponent {
        XMFLOAT3 velocity{0,0,0}, angularVelocity{0,0,0};
        float mass{1};
        bool isStatic{false};
    };

    struct NameComponent {
        std::string name;
        NameComponent()=default;
        NameComponent(const std::string& n):name(n){}
    };

    struct TagComponent {
        std::string tag;
        TagComponent()=default;
        TagComponent(const std::string& t):tag(t){}
    };
}
