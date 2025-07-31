#pragma once
#include "../EditorManager.h"
#include "../../ECS/Components.h"
#include <imgui.h>

namespace SparkEngine {
    class InspectorWindow : public EditorWindow {
    private:
        EntityRegistry* m_registry;
        Entity m_selectedEntity = entt::null;

    public:
        InspectorWindow(EntityRegistry* registry) : m_registry(registry) {}

        void Render() override {
            if (!m_visible) return;

            if (ImGui::Begin("Inspector", &m_visible)) {
                if (m_selectedEntity != entt::null && m_registry->IsValid(m_selectedEntity)) {
                    RenderEntityInspector(m_selectedEntity);
                } else {
                    ImGui::Text("No entity selected");
                }
            }
            ImGui::End();
        }

        void OnEntitySelected(Entity entity) override {
            m_selectedEntity = entity;
        }

        const char* GetName() const override { return "Inspector"; }

    private:
        void RenderEntityInspector(Entity entity) {
            // Entity header
            ImGui::Text("Entity: %d", (int)entity);
            ImGui::Separator();

            // Name Component
            if (m_registry->HasComponent<NameComponent>(entity)) {
                if (ImGui::CollapsingHeader("Name Component", ImGuiTreeNodeFlags_DefaultOpen)) {
                    auto& nameComp = m_registry->GetComponent<NameComponent>(entity);
                    char buffer[256];
                    strcpy_s(buffer, nameComp.name.c_str());
                    if (ImGui::InputText("Name", buffer, sizeof(buffer))) {
                        nameComp.name = buffer;
                    }
                }
            }

            // Transform Component
            if (m_registry->HasComponent<TransformComponent>(entity)) {
                if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
                    auto& transform = m_registry->GetComponent<TransformComponent>(entity);

                    ImGui::DragFloat3("Position", &transform.position.x, 0.1f);

                    DirectX::XMFLOAT3 rotationDegrees = {
                        DirectX::XMConvertToDegrees(transform.rotation.x),
                        DirectX::XMConvertToDegrees(transform.rotation.y),
                        DirectX::XMConvertToDegrees(transform.rotation.z)
                    };
                    if (ImGui::DragFloat3("Rotation", &rotationDegrees.x, 1.0f)) {
                        transform.rotation.x = DirectX::XMConvertToRadians(rotationDegrees.x);
                        transform.rotation.y = DirectX::XMConvertToRadians(rotationDegrees.y);
                        transform.rotation.z = DirectX::XMConvertToRadians(rotationDegrees.z);
                    }

                    ImGui::DragFloat3("Scale", &transform.scale.x, 0.1f, 0.1f, 10.0f);
                }
            }

            // Camera Component
            if (m_registry->HasComponent<CameraComponent>(entity)) {
                if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
                    auto& camera = m_registry->GetComponent<CameraComponent>(entity);

                    ImGui::DragFloat("FOV", &camera.fov, 1.0f, 10.0f, 120.0f);
                    ImGui::DragFloat("Near Plane", &camera.nearPlane, 0.01f, 0.01f, 10.0f);
                    ImGui::DragFloat("Far Plane", &camera.farPlane, 1.0f, 10.0f, 10000.0f);
                    ImGui::Checkbox("Primary Camera", &camera.isPrimary);
                }
            }

            // Light Component
            if (m_registry->HasComponent<LightComponent>(entity)) {
                if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen)) {
                    auto& light = m_registry->GetComponent<LightComponent>(entity);

                    const char* types[] = { "Directional", "Point", "Spot" };
                    int currentType = (int)light.type;
                    if (ImGui::Combo("Type", &currentType, types, 3)) {
                        light.type = (LightComponent::Type)currentType;
                    }

                    ImGui::ColorEdit3("Color", &light.color.x);
                    ImGui::DragFloat("Intensity", &light.intensity, 0.1f, 0.0f, 10.0f);

                    if (light.type != LightComponent::Directional) {
                        ImGui::DragFloat("Range", &light.range, 0.1f, 0.1f, 100.0f);
                    }

                    if (light.type == LightComponent::Spot) {
                        ImGui::DragFloat("Spot Angle", &light.spotAngle, 1.0f, 1.0f, 90.0f);
                    }

                    ImGui::Checkbox("Cast Shadows", &light.castShadows);
                }
            }

            // Add Component button
            ImGui::Separator();
            if (ImGui::Button("Add Component")) {
                ImGui::OpenPopup("AddComponentPopup");
            }

            if (ImGui::BeginPopup("AddComponentPopup")) {
                if (!m_registry->HasComponent<NameComponent>(entity) && ImGui::MenuItem("Name Component")) {
                    m_registry->AddComponent<NameComponent>(entity);
                }
                if (!m_registry->HasComponent<TransformComponent>(entity) && ImGui::MenuItem("Transform Component")) {
                    m_registry->AddComponent<TransformComponent>(entity);
                }
                if (!m_registry->HasComponent<CameraComponent>(entity) && ImGui::MenuItem("Camera Component")) {
                    m_registry->AddComponent<CameraComponent>(entity);
                }
                if (!m_registry->HasComponent<LightComponent>(entity) && ImGui::MenuItem("Light Component")) {
                    m_registry->AddComponent<LightComponent>(entity);
                }
                if (!m_registry->HasComponent<MeshRendererComponent>(entity) && ImGui::MenuItem("Mesh Renderer")) {
                    m_registry->AddComponent<MeshRendererComponent>(entity);
                }
                ImGui::EndPopup();
            }
        }
    };
}
