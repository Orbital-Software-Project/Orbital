
#include "Editor/Views/Toolbar.h"
#include "Editor/Utils.hpp"

#include "Editor/Tasks/MeshImportTask.h"
#include "Editor/Tasks/TaskWorker.h"

#include <string>
#include <imgui.h>

namespace Orb {

    Toolbar::Toolbar() {

    }

    Toolbar::~Toolbar() {

    }

    void Toolbar::OnRender() {

        // Main menubar
        {
            ImGui::BeginMainMenuBar();
            {
                if (ImGui::BeginMenu("File")) {
                    if (ImGui::MenuItem("Import")) {
                        std::string file = Utils::PickFile();
                        if (file != "") {
                            TaskWorker::GetInstance().Exec(std::make_shared<MeshImportTask>(file));
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

                    ImGui::EndMenu();
                }
            }
            ImGui::EndMainMenuBar();
        }

    }

}
