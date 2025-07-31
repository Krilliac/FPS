#pragma once
#include "../EditorManager.h"
#include "../../ECS/Components.h"
#include <imgui.h>

namespace SparkEngine {
    class HierarchyWindow : public EditorWindow {
    private:
        EntityRegistry* m_registry;
        Entity m_selectedEntity = entt::null;
        std::function<void(Entity)> m_onEntitySelected;
        char m_entityNameBuffer[256] = "New Entity";

    public:
        HierarchyWindow(EntityRegistry* registry, std::function<void(Entity)> onEntitySelected = nullptr) 
            : m_registry(registry), m_onEntitySelected(onEntitySelected) {}

        void Render() override {
            if (!m_visible) return;

            if (ImGui::Begin("Hierarchy", &m_visible)) {
                // Create entity button
                if (ImGui::Button("Create Entity")) {
                    Entity newEntity = m_registry->CreateEntity();
                    m_registry->AddComponent<NameComponent>(newEntity, std::string(m_entityNameBuffer));
                    m_registry->AddComponent<TransformComponent>(newEntity);
                }

                ImGui::SameLine();
                ImGui::InputText("##EntityName", m_entityNameBuffer, sizeof(m_entityNameBuffer));

                ImGui::Separator();

                // Entity list
                auto view = m_registry->View<NameComponent>();
                for (auto entity : view) {
                    auto& nameComp = view.get<NameComponent>(entity);

                    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | 
                                             ImGuiTreeNodeFlags_NoTreePushOnOpen |
                                             ImGuiTreeNodeFlags_SpanFullWidth;

                    if (entity == m_selectedEntity) {
                        flags |= ImGuiTreeNodeFlags_Selected;
                    }

                    bool nodeOpen = ImGui::TreeNodeEx(
                        (void*)(intptr_t)entity, 
                        flags, 
                        "%s", 
                        nameComp.name.c_str()
                    );

                    if (ImGui::IsItemClicked()) {
                        m_selectedEntity = entity;
                        if (m_onEntitySelected) {
                            m_onEntitySelected(entity);
                        }
                    }

                    // Context menu
                    if (ImGui::BeginPopupContextItem()) {
                        if (ImGui::MenuItem("Delete Entity")) {
                            m_registry->DestroyEntity(entity);
                        }
                        if (ImGui::MenuItem("Duplicate Entity")) {
                            // TODO: Implement entity duplication
                        }
                        ImGui::EndPopup();
                    }
                }
            }
            ImGui::End();
        }

        const char* GetName() const override { return "Hierarchy"; }
        Entity GetSelectedEntity() const { return m_selectedEntity; }
        void SetSelectedEntity(Entity entity) { m_selectedEntity = entity; }
    };
}
