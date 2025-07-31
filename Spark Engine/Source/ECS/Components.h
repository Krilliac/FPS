#pragma once
#include "../Core/framework.h"
#include <string>

namespace SparkEngine {
    // Transform Component
    struct TransformComponent {
        DirectX::XMFLOAT3 position = {0.0f, 0.0f, 0.0f};
        DirectX::XMFLOAT3 rotation = {0.0f, 0.0f, 0.0f};
        DirectX::XMFLOAT3 scale = {1.0f, 1.0f, 1.0f};

        DirectX::XMMATRIX GetWorldMatrix() const {
            DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
            DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
            DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
            return S * R * T;
        }
    };

    // Name Component
    struct NameComponent {
        std::string name = "Entity";

        NameComponent() = default;
        NameComponent(const std::string& n) : name(n) {}
    };

    // Mesh Renderer Component
    struct MeshRendererComponent {
        std::string meshPath;
        std::string materialPath;
        bool visible = true;
        uint32_t renderLayer = 0;

        MeshRendererComponent() = default;
        MeshRendererComponent(const std::string& mesh, const std::string& material) 
            : meshPath(mesh), materialPath(material) {}
    };

    // Camera Component
    struct CameraComponent {
        float fov = 45.0f;
        float nearPlane = 0.1f;
        float farPlane = 1000.0f;
        float aspectRatio = 16.0f / 9.0f;
        bool isPrimary = false;

        DirectX::XMMATRIX GetProjectionMatrix() const {
            return DirectX::XMMatrixPerspectiveFovLH(
                DirectX::XMConvertToRadians(fov),
                aspectRatio,
                nearPlane,
                farPlane
            );
        }

        DirectX::XMMATRIX GetViewMatrix(const TransformComponent& transform) const {
            DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&transform.position);
            DirectX::XMVECTOR forward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
            DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

            // Apply rotation
            DirectX::XMMATRIX rotMatrix = DirectX::XMMatrixRotationRollPitchYaw(
                transform.rotation.x, transform.rotation.y, transform.rotation.z);
            forward = DirectX::XMVector3TransformNormal(forward, rotMatrix);
            up = DirectX::XMVector3TransformNormal(up, rotMatrix);

            return DirectX::XMMatrixLookToLH(pos, forward, up);
        }
    };

    // Light Component
    struct LightComponent {
        enum Type { Directional, Point, Spot };

        Type type = Directional;
        DirectX::XMFLOAT3 color = {1.0f, 1.0f, 1.0f};
        float intensity = 1.0f;
        float range = 10.0f;  // For point/spot lights
        float spotAngle = 45.0f;  // For spot lights
        bool castShadows = false;
    };
}