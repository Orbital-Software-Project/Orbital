#include "PropertyEditor.h"

#include <imgui.h>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtc/quaternion.hpp>
#include<glm/gtx/quaternion.hpp>

#include "Global.h"

namespace Orb {

PropertyEditor::PropertyEditor() {}

PropertyEditor::~PropertyEditor() {}

void PropertyEditor::OnRender() {

    ImGui::Begin("Property Editor");
    {
        auto &meshList = Global::GetInstance().MeshProperties;
        std::shared_ptr<Mesh> meshToRemove = nullptr;


        for(int i = 0; i < meshList.size(); i++) {


            ImGui::PushID(std::to_string(i).c_str());
            {

                auto currMesh = meshList[i];
                glm::mat4 meshModelMat = currMesh->GetModel();

                ImGui::Text("Model Matrix");
                ImGui::SameLine(ImGui::GetWindowWidth()-30);

                if(ImGui::Button("X")) {
                    meshToRemove = currMesh;
                }

                static glm::vec3 translationPrevFrame;

                glm::vec3 scale;
                glm::quat rotation;
                glm::vec3 translation;
                glm::vec3 skew;
                glm::vec4 perspective;

                glm::decompose(meshModelMat, scale, rotation, translation, skew, perspective);

                // conjugate rotation quaternion
                rotation = glm::conjugate(rotation);

                glm::vec3 angles = glm::eulerAngles(rotation);



                ImGui::InputFloat3("Tr", glm::value_ptr(translation), 3);
                if(ImGui::IsItemEdited()) {
                    static glm::vec3 translationPrevFrame;
                    if(translation != translationPrevFrame) {
                        meshModelMat = glm::translate(glm::mat4(1.0f), translation);
                        currMesh->SetModel(meshModelMat);
                    }
                    translationPrevFrame = translation;
                }



                ImGui::InputFloat3("Rt", glm::value_ptr(angles), 3);
                if(ImGui::IsItemEdited()) {
                    static glm::vec3 rotationPrevFrame;
                    if(rotationPrevFrame != angles) {

                        // TODO: http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
                        //glm::quat(angles);

                        meshModelMat = glm::rotate(meshModelMat, angles.x, glm::vec3(1.0f, 0.0f, 0.0f));
                        meshModelMat = glm::rotate(meshModelMat, angles.y, glm::vec3(0.0f, 1.0f, 0.0f));
                        meshModelMat = glm::rotate(meshModelMat, angles.z, glm::vec3(0.0f, 0.0f, 1.0f));

                        //meshModelMat = meshModelMat * glm::toMat4(rotation);
                        currMesh->SetModel(meshModelMat);
                    }
                    rotationPrevFrame = angles;
                }



                if(ImGui::InputFloat3("Sc", glm::value_ptr(scale), 3)) {
                    if(scale.x != 0 && scale.y != 0 && scale.z != 0) {
                        static glm::vec3 scalePrevFrame;
                        if(scalePrevFrame != scale) {
                            meshModelMat = glm::scale(meshModelMat, scale);
                            currMesh->SetModel(meshModelMat);
                        }
                        scalePrevFrame = scale;
                    }
                }

                ImGui::Separator();

            }
            ImGui::PopID();
        }


        if(meshToRemove != nullptr) {
            meshList.erase(std::remove(meshList.begin(), meshList.end(), meshToRemove), meshList.end());
            meshToRemove = nullptr;
        }

    }
    ImGui::End();
}

void PropertyEditor::matrix4x4(glm::mat4 mat) {

}

}
