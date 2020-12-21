#include "Outliner.h"

#include <imgui.h>

#include "Editor/Global.h"

namespace Orb {

Outliner::Outliner(std::shared_ptr<SceneRenderer> renderer) {
    this->renderer = renderer;
}

Outliner::~Outliner() {

}

void Outliner::OnRender(){

    ImGui::Begin("Outliner");
    {
        auto entities = renderer->GetEntities();
        int i = 0;
        // Display all meshes
        {

            std::shared_ptr<IEntity> entityToRemove = nullptr;
            for(std::shared_ptr<IEntity> entity : entities) {
                if (!entity->VisibleInOutliner) {
                    continue;
                }

                ImGui::PushID(std::to_string(i++).c_str());

                ImGui::Text(entity->Name.c_str());
                ImGui::SameLine(100);
                if(ImGui::Button("Remove")) {
                    entityToRemove = entity;
                }

                ImGui::SameLine();
                if(ImGui::Button(entity->Visible ? "Hide" : "Show")) {
                    entity->Visible = !entity->Visible;
                }

                ImGui::SameLine();
                if(ImGui::Button("Properties")) {
                    entity->Selected = true;
                }

                ImGui::PopID();
            }
            // Remove mesh object
            if(entityToRemove != nullptr) {
                this->renderer->RemoveEntity(entityToRemove);
                entityToRemove = nullptr;
            }
        }




    }
    ImGui::End();


}

}
