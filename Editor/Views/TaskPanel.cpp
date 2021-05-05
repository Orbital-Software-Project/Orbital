#include "TaskPanel.h"

#include "Editor/Utils.hpp"



#include <imgui.h>

#include "Editor/Tasks/TaskWorker.h"
//#include "Editor/Tasks/Viso2Task.h"
#include "Editor/Tasks/OpenVslamTask.h"

#include "Editor/Global.h"

namespace Orb {

    TaskPanel::TaskPanel() {
        this->vocabFile = Global::GetInstance().RootDir + "\\openvslam\\3rd\\FBoW\\vocabulary\\orb_vocab.fbow";
    }

    TaskPanel::~TaskPanel() {

    }

    void TaskPanel::OnRender() {

        if (ImGui::Begin("Task Panel", &this->Open)) {

            // Filepicker
            if (ImGui::Button("Video")) {
                videoFile = Utils::PickFile();
            }

            ImGui::Text("%s", videoFile.c_str());

            if (ImGui::Button("Vocab")) {
                vocabFile = Utils::PickFile();
            }

            ImGui::Text("%s", vocabFile.c_str());

            if (ImGui::Button("Cfg")) {
                cfgFile = Utils::PickFile();
            }

            ImGui::Text("%s", cfgFile.c_str());

            // SLAM can only be started when files are present
            if (videoFile != "" && cfgFile != "" && vocabFile != "") {
                if (ImGui::Button("Run SLAM")) {

                    TaskWorker::GetInstance().Exec(std::make_shared<OpenVslamTask>(videoFile, cfgFile, vocabFile));

                }


            }

            if (videoFile != "") {
                if (ImGui::Button("Run libViso2 SLAM")) {
                    /*TaskWorker::GetInstance().Exec(
                        std::make_shared<Viso2Task>(Viso2TaskParam(959.500000, 539.500000, 1034.650696, videoFile))
                    );*/
                }
            }

            {
                ImGui::Text("Current Tasks", "");

                ImGui::Separator();

                auto running_tasks = TaskWorker::GetInstance().GetRunningTasks();
                int id = 0;

                for (auto task : running_tasks) {
                    ImGui::PushID(std::to_string(id++).c_str());
                    {
                        ImGui::Text("Task %i", id);
                        ImGui::SameLine();
                        ImGui::ProgressBar(
                            static_cast<float>(
                                task->GetReport()->Percentage.GetCurrStep()) / static_cast<float>(task->GetReport()->Percentage.GetMaxSteps())
                        );
                    }
                    ImGui::PopID();
                }
            }


            ImGui::End();
        }

    }
}
