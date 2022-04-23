
#include "Editor/Views/Toolbar.h"
#include "Editor/Utils.hpp"

#include "Editor/Tasks/MeshImportTask.h"
#include "Editor/Tasks/TaskWorker.h"

#include <string>
#include <imgui.h>

// Import video
#include "Editor/Global.h"
#include "Editor/Engine/Video.h"

namespace Orb {

    Toolbar::Toolbar() {

        this->Name = "Toolbar";
        this->Type = ViewType::ViewType_Toolbar;
        
    }

    Toolbar::~Toolbar() {

    }

    void Toolbar::OnRender() {

        // Main menubar
        {
            ImGui::BeginMainMenuBar();
            {
                if (ImGui::BeginMenu("File")) {
                    if (ImGui::MenuItem("Import mesh")) {
                        std::string file = Utils::PickFile();
                        if (file != "") {
                            TaskWorker::GetInstance().Exec(std::make_shared<MeshImportTask>(file));
                        }
                    }

                    if (ImGui::MenuItem("Import video")) {
                        std::string file = Utils::PickFile();
                        if (file != "") {
                            EditorState::GetInstance().Renderer->AddEntity(std::make_shared<Video>(file));
                        }
                    }

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Views")) {
                    if (ImGui::MenuItem("Map viewer")) {
                        this->Request = ViewWindowRequest::Open_MapViewer;
                    }
                    if (ImGui::MenuItem("Video preview")) {
                        this->Request = ViewWindowRequest::Open_VideoPrev;
                    }
                    if (ImGui::MenuItem("Sequencer")) {
                        this->Request = ViewWindowRequest::Open_Sequencer;
                    }
                    if (ImGui::MenuItem("Property editor")) {
                        this->Request = ViewWindowRequest::Open_PropertyEd;
                    }
                    if (ImGui::MenuItem("Task panel")) {
                        this->Request = ViewWindowRequest::Open_Taskpanel;
                    }
                    if (ImGui::MenuItem("Outliner")) {
                        this->Request = ViewWindowRequest::Open_Outliner;
                    }
                    if (ImGui::MenuItem("Node editor")) {
                        this->Request = ViewWindowRequest::Open_NodeEd;
                    }

                    ImGui::EndMenu();
                }
            }
            ImGui::EndMainMenuBar();
        }

    }

}
