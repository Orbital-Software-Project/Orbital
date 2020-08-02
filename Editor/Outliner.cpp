#include "Outliner.h"

#include <imgui.h>

namespace Orb {

Outliner::Outliner(std::shared_ptr<SceneRenderer> renderer) {
    this->renderer = renderer;
}

Outliner::~Outliner() {

}

void Outliner::OnRender(){

    ImGui::Begin("Outliner");
    {
        auto meshes = renderer->GetMeshes();
        auto cameras = renderer->GetCameras();
        int i = 0;
        // Display all meshes
        {

            std::shared_ptr<Mesh> meshToRemove = nullptr;
            for(std::shared_ptr<Mesh> mesh : meshes) {
                ImGui::PushID(std::to_string(i++).c_str());

                ImGui::Text("Mesh");
                ImGui::SameLine();
                if(ImGui::Button("Remove")) {
                    meshToRemove = mesh;
                }
                ImGui::SameLine();
                if(ImGui::Button("Hide")) {
                    mesh->ToggleHide();
                }

                ImGui::PopID();
            }
            // Remove mesh object
            if(meshToRemove != nullptr) {
                this->renderer->RemoveMesh(meshToRemove);
                meshToRemove = nullptr;
            }
        }


        // Display all cameras
        {
            std::shared_ptr<Camera> cameraToRemove = nullptr;
            for(std::shared_ptr<Camera> camera : cameras) {

                ImGui::PushID(std::to_string(i++).c_str());

                ImGui::Text("Camera");
                ImGui::SameLine();
                if(ImGui::Button("Remove")) {
                    cameraToRemove = camera;
                }
                ImGui::SameLine();
                if(ImGui::Button("Hide")) {
                    camera->ToggleHide();
                }

                ImGui::PopID();
            }
            if(cameraToRemove != nullptr) {
                this->renderer->RemoveCamera(cameraToRemove);
                cameraToRemove = nullptr;
            }
        }




    }
    ImGui::End();


}

}
