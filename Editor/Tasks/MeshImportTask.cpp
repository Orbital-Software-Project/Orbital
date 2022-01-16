#include "MeshImportTask.h"

#include "Editor/Global.h"
#include "Engine/MeshImporter.h"

#include "Editor/Base/ScopeMutexLock.hpp"

namespace Orb {

    MeshImportTask::MeshImportTask(MeshImportTaskParam param) {
        this->param = param;
    }

    MeshImportTask::~MeshImportTask() {
        this->Cancel();
    }

    void MeshImportTask::Run() {
        this->Status = TaskStatus::Running;

        MeshImporter exporter;
        auto lst = exporter.Import(this->param.FilePath);


        if (this->CancelRequested) {
            this->Status = TaskStatus::Canceled;
            return;
        }

        for (std::shared_ptr<Mesh> meshdata : lst) {
            meshdata->DrawOnlyVertColors(false);
            EditorState::GetInstance().Renderer->AddEntity(std::move(meshdata));
        }

        this->Status = TaskStatus::Finished;


        // Last report
        {
            ScopeMutexLock lock(this->TaskReportMutex);

            this->taskReport.Percentage = Percentage(100, 100);
        }

    }

    MeshImportTask::ITaskReport* MeshImportTask::GetReport() {
        ScopeMutexLock lock(this->TaskReportMutex);

        return &this->taskReport;
    }

}


