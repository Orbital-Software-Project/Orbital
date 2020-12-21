#include "TaskPanel.h"

#include "Editor/Utils.hpp"



#include <imgui.h>

#include "Editor/Tasks/TaskWorker.h"
#include "Editor/Tasks/Viso2Task.h"
#include "Editor/Tasks/OpenVslamTask.h"

namespace Orb {

    TaskPanel::TaskPanel() {

    }

    TaskPanel::~TaskPanel() {

    }

    void TaskPanel::OnRender() {

        this->processfinishedTasks();

        ImGui::Begin("Task Panel");

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


                //slamTask = std::make_unique<SlamTask>(videoFile, cfgFile, vocabFile);
                //slamTask->Run();

                //Global::GetInstance().FramePublisher = slamTask->GetFramePublisher();
                //Global::GetInstance().MapPublisher = slamTask->GetMapPublisher();

            }

            



            //if (slamTask) {
            //    TaskReport report = slamTask->GetReport();
            //    // Show progressbar and cancel button only when slam is running
            //    if (report.Status == TaskStatus::Running) {
            //        ImGui::ProgressBar(static_cast<float>(report.FramesProcessed) / static_cast<float>(report.NumFrames));
            //        if (ImGui::Button("Cancel")) {
            //            slamTask->Cancel();
            //        }
            //    }
            //}

        }

        if (videoFile != "") {
            if (ImGui::Button("Run libViso2 SLAM")) {
                
                TaskWorker::GetInstance().Exec(
                    std::make_shared<Viso2Task>(Viso2TaskParam(959.500000, 539.500000, 1034.650696, videoFile))
                );

                //slamTask = std::make_unique<SlamTask>(videoFile, cfgFile, vocabFile);
                //slamTask->Run(true);

                //Global::GetInstance().FramePublisher = slamTask->GetFramePublisher();
                //Global::GetInstance().MapPublisher = slamTask->GetMapPublisher();

            }
        }

        {
            ImGui::LabelText("Current Tasks", "");

            ImGui::Separator();

            auto& running_tasks = TaskWorker::GetInstance().GetRunningTasks();
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

    void TaskPanel::processfinishedTasks() {
        auto tasks = TaskWorker::GetInstance().GetFinishedTasks();
        for (std::shared_ptr<ITask> task : tasks) {



        }
    }
}
