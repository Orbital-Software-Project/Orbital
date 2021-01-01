#pragma once

#include "TaskWorker.h"

#include <string>

namespace Orb {

    struct MeshImportTaskParam {

        MeshImportTaskParam() : MeshImportTaskParam("") {}

        MeshImportTaskParam(std::string filePath) : FilePath(filePath) {}

        std::string FilePath = "";
    };

    class MeshImportTask : public ITask {
    public:
        struct MeshImportTaskReport : public ITask::ITaskReport {

        };

        MeshImportTask(MeshImportTaskParam param);

        ~MeshImportTask();

        void Run();

        ITaskReport* GetReport();

    private:
        MeshImportTaskParam param;
        MeshImportTaskReport taskReport;

    };
}
