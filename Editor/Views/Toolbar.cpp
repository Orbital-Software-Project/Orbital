
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
            }
            ImGui::EndMainMenuBar();
        }

    }

}
