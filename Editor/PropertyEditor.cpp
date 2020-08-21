#include "PropertyEditor.h"

#include <imgui.h>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtc/quaternion.hpp>
#include<glm/gtx/quaternion.hpp>

#include <glm/gtx/euler_angles.hpp>

#include "Global.h"

namespace Orb {

PropertyEditor::PropertyEditor(std::shared_ptr<SceneRenderer> renderer) {
    this->renderer = renderer;
}

PropertyEditor::~PropertyEditor() {}

void PropertyEditor::OnRender() {

    ImGui::Begin("Property Editor");
    {
        auto meshList = this->renderer->GetEntities();


        for(int i = 0; i < meshList.size(); i++) {
            auto currMesh = meshList[i];
            if(!currMesh->Selected) {
                continue;
            }

            ImGui::PushID(i);
            {
                ImGui::SameLine(ImGui::GetWindowWidth() - 30);

                if(ImGui::Button("X")) {
                    currMesh->Selected = false;
                }

                // --------------------

                ImGui::Text("Entity Model Matrix");

                ImGui::Text("%f %f %f %f", currMesh->Matrix[0][0], currMesh->Matrix[1][0], currMesh->Matrix[2][0], currMesh->Matrix[3][0]);
                ImGui::Text("%f %f %f %f", currMesh->Matrix[0][1], currMesh->Matrix[1][1], currMesh->Matrix[2][1], currMesh->Matrix[3][1]);
                ImGui::Text("%f %f %f %f", currMesh->Matrix[0][2], currMesh->Matrix[1][3], currMesh->Matrix[2][2], currMesh->Matrix[3][2]);
                ImGui::Text("%f %f %f %f", currMesh->Matrix[0][3], currMesh->Matrix[1][3], currMesh->Matrix[2][3], currMesh->Matrix[3][3]);

                // --------------------

                ImGui::DragFloat3("Tr", glm::value_ptr(currMesh->Position), 0.01f);
                ImGui::DragFloat3("Rt", glm::value_ptr(currMesh->Rotation), 0.01f);
                ImGui::DragFloat3("Sc", glm::value_ptr(currMesh->Scale),    0.01f);

                currMesh->Matrix =
                        glm::translate(glm::mat4(1.0), currMesh->Position) *
                        glm::yawPitchRoll(glm::radians(currMesh->Rotation.x), glm::radians(currMesh->Rotation.y), glm::radians(currMesh->Rotation.z)) *
                        glm::scale(glm::mat4(1.0f), currMesh->Scale);


                // --------------------

                ImGui::Separator();

            }

            ImGui::PopID();
        }


    }
    ImGui::End();
}

void PropertyEditor::matrix4x4(glm::mat4 mat) {

}

}
