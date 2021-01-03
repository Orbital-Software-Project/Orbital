#include "Outliner.h"

#include <imgui.h>

#include "Editor/Global.h"

namespace Orb {

    Outliner::Outliner(std::shared_ptr<SceneRenderer> renderer) {
        this->renderer = renderer;
    }

    Outliner::~Outliner() {

    }

    void Outliner::OnRender() {

        ImGui::Begin("Outliner");
        {

            auto entities = renderer->GetEntities();
            int i = 0;

            // Display all meshes
            {
                this->drawEntityTree(i, entities);
            }

        }
        ImGui::End();


    }

    void Outliner::drawEntityTree(int id, std::vector<std::shared_ptr<IEntity>> entities) {

        std::shared_ptr<IEntity> entityToRemove = nullptr;
        for (std::shared_ptr<IEntity> entity : entities) {
            
            if (!entity->VisibleInOutliner) {
                continue;
            }

            

            {
                ImGui::PushID(std::to_string(id++).c_str());

                if (entity->GetEntityType() == IEntity::EntityType::Group) {
                    auto childEntites = std::dynamic_pointer_cast<EntityGroup, IEntity>(entity)->GetChildEntites();
                    this->drawEntityTree(id, childEntites);
                }


                ImGui::Text(entity->Name.c_str());
                ImGui::SameLine(100);
                if (ImGui::Button("Remove")) {
                    entityToRemove = entity;
                }

                ImGui::SameLine();

                if (ImGui::Button(entity->Visible ? "Hide" : "Show")) {
                    if (entity->GetEntityType() == IEntity::EntityType::Group) {
                        std::dynamic_pointer_cast<EntityGroup, IEntity>(entity)->SetVisibility(!entity->Visible);
                    } else {
                        entity->Visible = !entity->Visible;
                    }
                }

                ImGui::SameLine();
                if (ImGui::Button("Properties")) {
                    entity->Selected = true;
                }

                ImGui::PopID();
            }



        }

        // Remove mesh object
        if (entityToRemove != nullptr) {
            this->renderer->RemoveEntity(entityToRemove);
            entityToRemove = nullptr;
        }

    }

}
